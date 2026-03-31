#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "read.hpp"
#include "matrix.hpp"
#include "recommend.hpp"
#include <ctime>

namespace py = pybind11;

std::vector<std::vector<float>> calcular_similaridade(Historico &historico, int fast) {
    int n = historico.clientes.size();
    FloatMatrix *matrixSim = getFloatMatrix(n, n);
    if (!matrixSim) {
        throw std::runtime_error("Falha ao alocar matrixSim");
    }

    if (processMatrices(matrixSim, &historico, fast)) {
        freeFloatMatrix(matrixSim);
        throw std::runtime_error("Falha em processMatrices");
    }

    std::vector<std::vector<float>> resultado(n, std::vector<float>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            resultado[i][j] = matrixSim->elements[i][j];
        }
    }

    freeFloatMatrix(matrixSim);
    return resultado;
}

std::vector<std::pair<int, float>> recomendar_produtos(
    Historico &historico,
    std::vector<std::vector<float>> &matrixSim,
    int idCliente,
    int k
) {
    int n = matrixSim.size();
    FloatMatrix *matrix = getFloatMatrix(n, n);
    if (!matrix) {
        throw std::runtime_error("Falha ao alocar matrix");
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix->elements[i][j] = matrixSim[i][j];
        }
    }

    std::vector<Produto> recomendados;
    if (processRecommend(&recomendados, idCliente, matrix, &historico, k)) {
        freeFloatMatrix(matrix);
        throw std::runtime_error("Falha em processRecommend");
    }

    freeFloatMatrix(matrix);

    std::vector<std::pair<int, float>> resultado;
    for (const auto &p : recomendados) {
        resultado.push_back({p.idProduto, p.ranking});
    }
    return resultado;
}

std::pair<double, double> benchmark(Historico &historico) {
    int n = historico.clientes.size();

    FloatMatrix *matrixSim0 = getFloatMatrix(n, n);
    FloatMatrix *matrixSim1 = getFloatMatrix(n, n);
    if (!matrixSim0 || !matrixSim1) {
        freeFloatMatrix(matrixSim0);
        freeFloatMatrix(matrixSim1);
        throw std::runtime_error("Falha ao alocar matrizes");
    }

    clock_t t1 = clock();
    processMatrices(matrixSim0, &historico, 0);
    clock_t t2 = clock();
    processMatrices(matrixSim1, &historico, 1);
    clock_t t3 = clock();

    freeFloatMatrix(matrixSim0);
    freeFloatMatrix(matrixSim1);

    double tempo_padrao = (double)(t2 - t1) / CLOCKS_PER_SEC;
    double tempo_otimizado = (double)(t3 - t2) / CLOCKS_PER_SEC;

    return {tempo_padrao, tempo_otimizado};
}

PYBIND11_MODULE(pyrecommend, m) {
    py::class_<Historico>(m, "Historico")
        .def(py::init<>())
        .def_readwrite("clientes", &Historico::clientes)
        .def_readwrite("mapClientes", &Historico::mapClientes)
        .def_readwrite("produtos", &Historico::produtos)
        .def_readwrite("mapProdutos", &Historico::mapProdutos)
        .def_readwrite("listaCompras", &Historico::listaCompras);

    m.def("calcular_similaridade", &calcular_similaridade,
          py::arg("historico"), py::arg("fast") = 0);

    m.def("recomendar_produtos", &recomendar_produtos,
          py::arg("historico"), py::arg("matrixSim"),
          py::arg("idCliente"), py::arg("k"));

    m.def("benchmark", &benchmark,
          py::arg("historico"));
}
