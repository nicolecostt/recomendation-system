#include "matrix.hpp"

Matrix *getMatrix(int linhas, int colunas) {
  Matrix *matrix = malloc(sizeof(Matrix));
  if (matrix == NULL) {
    return NULL;
  }
  matrix->elements = malloc(linhas * sizeof(int *));
  if (matrix->elements == NULL) {
    return NULL;
  }
  for (int i = 0; i < linhas; i++) {
    matrix->elements[i] = malloc(colunas * sizeof(int));
    if (matrix->elements[i] == NULL) {
      return NULL;
    }
    for (int j = 0; j < colunas; j++) {
      matrix->elements[i][j] = 0;
    }
  }
  return matrix;
}

int getMatrixCompras(Matrix *matrixCompras, Historico *historico) {
  for (int i = 0; i < matrixCompras->linhas; i++) {
    for (int j: historico->listaCompras[i]) {
      matrixCompras->elements[i][j] = 1;
    }
  }
  return 0;
}

int transposeMatrix(Matrix *matrixT, Matrix *matrix) {
  if (matrixT->linhas != matrix->colunas || matrixT->colunas != matrix->linhas) {
    return 1;
  }
  for (int i = 0; i < matrix->linhas; i++) {
    for (int j = 0; j < matrix->colunas; j++) {
      matrixT->elements[j][i] = matrix->elements[i][j];
    }
  }
  return 0;
}

int matrixMultiply(Matrix *matrixProd, Matrix *matrix1, int **matrix2) {
  if (matrix2->linhas != matrix1->colunas) {
    return 1;
  }
  for (int i = 0; i < matrix1->linhas; i++) {
    for (int j = 0; j < matrix2->colunas; j++) {
      for (int k = 0; k < matrix1->colunas; k++) {
        matrixProd->elements[i][j] += matrix1->elements[i][k] * matrix2->elements[k][j];
      }
    }
  }
  return 0;
}

int getMatrixSim(Matrix *matrixSim, Matrix *matrixInter) {
  for (int i = 0; i < matrixInter->linhas; i++) {
    for (int j = 0; j < matrixInter->colunas; j++) {
      matrixSim->elements[i][j] = 1 - matrixInter->elements[i][j] / matrixInter->elements[i][i];
    }
  }
}
