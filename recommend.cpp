#include "recommend.hpp"
#include <stdio.h>
#include <new>
#include <algorithm>

static bool compararRanking(const Produto &a, const Produto &b) {
  return a.ranking < b.ranking;
}

int getVizinhos(std::list<int> *vizinhos, int idCliente, FloatMatrix *matrixSim) {
  for (int i = 0; i < matrixSim->linhas; i++) {
    if (i != idCliente && matrixSim->elements[idCliente][i] < 1) {
      vizinhos->push_back(i);
    }
  }

  return 0;
}

int getRanking(std::vector<float> *R, int idCliente, std::list<int> *vizinhos, FloatMatrix *matrixSim, Historico *historico) {
  int comprou;

  for (int i : *vizinhos) {
    for (int j = 0; j < (int)R->size(); j++) {
      comprou = 0;

      for (int k : historico->listaCompras[idCliente]) {
        if (k == j) {
          comprou = 1;
          break;
        }
      }

      if (comprou) {
        continue;
      }

      (*R)[j] *= matrixSim->elements[idCliente][i];
    }
  }

  return 0;
}

int getRecomendados(std::vector<Produto> *recomendados, std::vector<float> *R, int k) {
  std::vector<Produto> produtos;
  Produto produto;

  for (int i = 0; i < (int)R->size(); i++) {
    produto.idProduto = i;
    produto.ranking = (*R)[i];
    produtos.push_back(produto);
  }

  std::sort(produtos.begin(), produtos.end(), compararRanking);

  for (int i = 0; i < k && i < (int)produtos.size(); i++) {
    recomendados->push_back(produtos[i]);
  }

  return 0;
}

int processRecommend(std::vector<Produto> *recomendados, int idCliente, FloatMatrix *matrixSim, Historico *historico, int k) {
  std::list<int> *vizinhos = new (std::nothrow) std::list<int>;
  if (!vizinhos) {
    return 1;
  }

  getVizinhos(vizinhos, idCliente, matrixSim);

  std::vector<float> *R = new (std::nothrow) std::vector<float>(historico->produtos.size(), 1.0f);
  if (!R) {
    delete vizinhos;
    return 1;
  }

  getRanking(R, idCliente, vizinhos, matrixSim, historico);
  getRecomendados(recomendados, R, k);

  delete R;
  delete vizinhos;

  return 0;
}
