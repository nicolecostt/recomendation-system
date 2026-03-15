#ifndef MATRIX
#define MATRIX

#include "read.hpp"

typedef struct {
  int linhas;
  int colunas;
  int **elements;
} Matrix;

Matrix *getMatrix(int linhas, int colunas);

int getMatrixCompras(Matrix *matrixCompras, Historico *historico);
int transposeMatrix(Matrix *matrixT, Matrix *matrix);
int matrixMultiply(Matrix *matrixProd, Matrix *matrix1, Matrix *matrix2);
int getMatrixSim(Matrix *matrixSim, Matrix *matrixInter);

#endif
