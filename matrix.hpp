#ifndef MATRIX
#define MATRIX

#include "read.hpp"

typedef struct {
  int linhas;
  int colunas;
  float **elements;
} FloatMatrix;

FloatMatrix *getFloatMatrix(int linhas, int colunas);

void freeFloatMatrix(FloatMatrix *matrix);

int getMatrixCompras(FloatMatrix *matrixCompras, Historico *historico);
int transposeMatrix(FloatMatrix *matrixT, FloatMatrix *matrix);
int matrixMultiply(FloatMatrix *matrixProd, FloatMatrix *matrix1, FloatMatrix *matrix2);
int getMatrixSim(FloatMatrix *matrixSim, FloatMatrix *matrixInter);

int fastMatrixInter(FloatMatrix *matrixInter, FloatMatrix *matrixCompras);

int processMatrices(FloatMatrix *matrixSim, Historico *historico, int fast);

#endif
