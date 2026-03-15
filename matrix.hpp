#ifndef MATRIX
#define MATRIX

#include "read.hpp"

typedef struct {
  int linhas;
  int colunas;
  float **elements;
} FloatMatrix;

typedef struct {
  int linhas;
  int colunas;
  int **elements;
} IntMatrix;

IntMatrix *getIntMatrix(int linhas, int colunas);
FloatMatrix *getFloatMatrix(int linhas, int colunas);

void freeIntMatrix(IntMatrix *matrix);
void freeFloatMatrix(FloatMatrix *matrix);

int getMatrixCompras(IntMatrix *matrixCompras, Historico *historico);
int transposeMatrix(IntMatrix *matrixT, IntMatrix *matrix);
int matrixMultiply(IntMatrix *matrixProd, IntMatrix *matrix1, IntMatrix *matrix2);
int getMatrixSim(FloatMatrix *matrixSim, IntMatrix *matrixInter);

int processMatrices(FloatMatrix *matrixSim, Historico *historico);

#endif
