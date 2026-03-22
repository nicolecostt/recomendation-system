#ifndef RECOMEND
#define RECOMEND

#include "read.hpp"
#include "matrix.hpp"
#include <vector>
#include <list>

typedef struct {
  int idProduto;
  float ranking;
} Produto;

int getVizinhos(std::list<int> *vizinhos, int idCliente, FloatMatrix *matrixSim);

int getRanking(std::vector<float> *R, int idCliente, std::list<int> *vizinhos, FloatMatrix *matrixSim, Historico *historico);

int getRecomendados(std::vector<Produto> *recomendados, std::vector<float> *R, int k);

int processRecommend(std::vector<Produto> *recomendados, int idCliente, FloatMatrix *matrixSim, Historico *historico, int k);

#endif
