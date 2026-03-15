#include <stdlib.h>
#include <stdio.h>
#include "matrix.hpp"

IntMatrix *getIntMatrix(int linhas, int colunas) {
  IntMatrix *matrix = (IntMatrix *)malloc(sizeof(IntMatrix));
  if (!matrix) {
    perror("getIntMatrix");
    return NULL;
  }

  matrix->linhas = linhas;
  matrix->colunas = colunas;

  matrix->elements = (int **)malloc(linhas * sizeof(int *));
  if (!matrix->elements) {
    free(matrix);
    perror("getIntMatrix");
    return NULL;
  }
  for (int i = 0; i < linhas; i++) {
    matrix->elements[i] = (int *)malloc(colunas * sizeof(int));
    if (!matrix->elements[i]) {
      for (int j = 0; j < i; j++) {
        free(matrix->elements[j]);
      }
      free(matrix->elements);
      free(matrix);
      perror("getIntMatrix");
      return NULL;
    }

    for (int j = 0; j < colunas; j++) {
      matrix->elements[i][j] = 0;
    }
  }
  return matrix;
}

FloatMatrix *getFloatMatrix(int linhas, int colunas) {
  FloatMatrix *matrix = (FloatMatrix *)malloc(sizeof(FloatMatrix));
  if (!matrix) {
    perror("getFloatMatrix");
    return NULL;
  }

  matrix->linhas = linhas;
  matrix->colunas = colunas;

  matrix->elements = (float **)malloc(linhas * sizeof(float *));
  if (!matrix->elements) {
    free(matrix);
    perror("getFloatMatrix");
    return NULL;
  }
  for (int i = 0; i < linhas; i++) {
    matrix->elements[i] = (float *)malloc(colunas * sizeof(float));
    if (!matrix->elements[i]) {
      for (int j = 0; j < i; j++) {
        free(matrix->elements[j]);
      }
      free(matrix->elements);
      free(matrix);
      perror("getFloatMatrix");
      return NULL;
    }

    for (int j = 0; j < colunas; j++) {
      matrix->elements[i][j] = 0;
    }
  }
  return matrix;
}

void freeIntMatrix(IntMatrix *matrix) {
  for (int i = 0; i < matrix->linhas; i++) {
    free(matrix->elements[i]);
  }
  free(matrix->elements);
  free(matrix);
  matrix = NULL;
}

void freeFloatMatrix(FloatMatrix *matrix) {
  for (int i = 0; i < matrix->linhas; i++) {
    free(matrix->elements[i]);
  }
  free(matrix->elements);
  free(matrix);
  matrix = NULL;
}

int getMatrixCompras(IntMatrix *matrixCompras, Historico *historico) {
  if (matrixCompras->linhas != historico->clientes.size() ||
      matrixCompras->colunas != historico->produtos.size()) {
    fprintf(stderr, "getMatrixCompras: Dimension mismatch.\n");
    return 1;
  }

  for (int i = 0; i < matrixCompras->linhas; i++) {
    for (int j: historico->listaCompras[i]) {
      matrixCompras->elements[i][j] = 1;
    }
  }

  return 0;
}

int transposeMatrix(IntMatrix *matrixT, IntMatrix *matrix) {
  if (matrixT->linhas != matrix->colunas ||
      matrixT->colunas != matrix->linhas) {
    fprintf(stderr, "transposeMatrix: Dimension mismatch.\n");
    return 1;
  }

  for (int i = 0; i < matrix->linhas; i++) {
    for (int j = 0; j < matrix->colunas; j++) {
      matrixT->elements[j][i] = matrix->elements[i][j];
    }
  }

  return 0;
}

int matrixMultiply(IntMatrix *matrixProd, IntMatrix *matrix1, IntMatrix *matrix2) {
  if (matrix2->linhas != matrix1->colunas ||
      matrixProd->linhas != matrix1->linhas ||
      matrixProd->colunas != matrix2->colunas) {
    fprintf(stderr, "matrixMultiply: Dimension mismatch.\n");
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

int getMatrixSim(FloatMatrix *matrixSim, IntMatrix *matrixInter) {
  if (matrixSim->linhas != matrixInter->linhas ||
      matrixSim->colunas != matrixInter->colunas) {
    fprintf(stderr, "getMatrixSim: Dimension mismatch.\n");
    return 1;
  }

  for (int i = 0; i < matrixInter->linhas; i++) {
    for (int j = 0; j < matrixInter->colunas; j++) {
      matrixSim->elements[i][j] = 1 - (matrixInter->elements[i][j] / (float) matrixInter->elements[i][i]);
    }
  }

  return 0;
}

int processMatrices(FloatMatrix *matrixSim, Historico *historico) {
  IntMatrix *matrixCompras = getIntMatrix(historico->clientes.size(), historico->produtos.size());
  if (!matrixCompras) {
    perror("processMatrices");
    return 1;
  }

  if (getMatrixCompras(matrixCompras, historico)) {
    freeIntMatrix(matrixCompras);
    fprintf(stderr, "processMatrices: matrixCompras failed.\n");
    return 1;
  }

  IntMatrix *matrixComprasT = getIntMatrix(matrixCompras->colunas, matrixCompras->linhas);
  if (!matrixComprasT) {
    perror("processMatrices");
    return 1;
  }

  if (transposeMatrix(matrixComprasT, matrixCompras)) {
    freeIntMatrix(matrixCompras);
    freeIntMatrix(matrixComprasT);
    fprintf(stderr, "processMatrices: transposeMatrix failed.\n");
    return 1;
  }

  IntMatrix *matrixInter = getIntMatrix(matrixCompras->linhas, matrixComprasT->colunas);
  if (!matrixInter) {
    perror("processMatrices");
    return 1;
  }

  if (matrixMultiply(matrixInter, matrixCompras, matrixComprasT)) {
    freeIntMatrix(matrixCompras);
    freeIntMatrix(matrixComprasT);
    freeIntMatrix(matrixInter);
    fprintf(stderr, "processMatrices: matrixMultiply failed.\n");
    return 1;
  }

  freeIntMatrix(matrixCompras);
  freeIntMatrix(matrixComprasT);

  if (getMatrixSim(matrixSim, matrixInter)) {
    freeIntMatrix(matrixInter);
    fprintf(stderr, "processMatrices: getMatrixSim failed.\n");
    return 1;
  }
  
  freeIntMatrix(matrixInter);

  return 0;
}
