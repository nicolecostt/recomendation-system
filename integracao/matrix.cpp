#include "matrix.hpp"
#include <stdio.h>
#include <stdlib.h>

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
    perror("getFloatMatrix");
    free(matrix);
    return NULL;
  }

  for (int i = 0; i < linhas; i++) {
    matrix->elements[i] = (float *)malloc(colunas * sizeof(float));
    if (!matrix->elements[i]) {
      perror("getFloatMatrix");
      for (int j = 0; j < i; j++) {
        free(matrix->elements[j]);
      }
      free(matrix->elements);
      free(matrix);
      return NULL;
    }

    for (int j = 0; j < colunas; j++) {
      matrix->elements[i][j] = 0;
    }
  }
  return matrix;
}

void freeFloatMatrix(FloatMatrix *matrix) {
  if (!matrix) {
    return;
  }

  for (int i = 0; i < matrix->linhas; i++) {
    free(matrix->elements[i]);
  }

  free(matrix->elements);
  free(matrix);
  matrix = NULL;
}

int getMatrixCompras(FloatMatrix *matrixCompras, Historico *historico) {
  if (matrixCompras->linhas != (int)historico->clientes.size() ||
      matrixCompras->colunas != (int)historico->produtos.size()) {
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

int fastMatrixInter(FloatMatrix *matrixInter, FloatMatrix *matrixCompras, Historico *historico) {
  if (matrixInter->linhas != matrixCompras->linhas ||
      matrixInter->colunas != matrixCompras->linhas) {
    fprintf(stderr, "fastMatrixInter: Dimension mismatch.\n");
    return 1;
  }

  for (int i = 0; i < matrixInter->linhas; i++) {
    matrixInter->elements[i][i] = historico->listaCompras[i].size();
    for (int j = 0; j < i; j++) {
      for (int k = 0; k < matrixCompras->colunas; k++) {
        matrixInter->elements[i][j] += matrixCompras->elements[i][k] * matrixCompras->elements[j][k];
      }
      matrixInter->elements[j][i] = matrixInter->elements[i][j];
    }
  }

  return 0;
}

int transposeMatrix(FloatMatrix *matrixT, FloatMatrix *matrix) {
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

int matrixMultiply(FloatMatrix *matrixProd, FloatMatrix *matrix1, FloatMatrix *matrix2) {
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

int getMatrixSim(FloatMatrix *matrixSim, FloatMatrix *matrixInter) {
  if (matrixSim->linhas != matrixInter->linhas ||
      matrixSim->colunas != matrixInter->colunas) {
    fprintf(stderr, "getMatrixSim: Dimension mismatch.\n");
    return 1;
  }

  for (int i = 0; i < matrixInter->linhas; i++) {
    if (matrixInter->elements[i][i] == 0) {
      continue;
    }
    for (int j = 0; j < matrixInter->colunas; j++) {
      matrixSim->elements[i][j] = 1 - (matrixInter->elements[i][j] / (float)matrixInter->elements[i][i]);
    }
  }

  return 0;
}

int processMatrices(FloatMatrix *matrixSim, Historico *historico, int fast) {
  FloatMatrix *matrixCompras = getFloatMatrix(historico->clientes.size(), historico->produtos.size());
  if (!matrixCompras) {
    perror("processMatrices");
    return 1;
  }

  if (getMatrixCompras(matrixCompras, historico)) {
    fprintf(stderr, "processMatrices: matrixCompras failed.\n");
    freeFloatMatrix(matrixCompras);
    return 1;
  }

  FloatMatrix *matrixInter = getFloatMatrix(matrixCompras->linhas, matrixCompras->linhas);
  if (!matrixInter) {
    perror("processMatrices");
    freeFloatMatrix(matrixCompras);
    return 1;
  }

  if (fast) {
    if (fastMatrixInter(matrixInter, matrixCompras, historico)) {
      fprintf(stderr, "processMatrices: fastMatrixInter failed.\n");
      freeFloatMatrix(matrixCompras);
      freeFloatMatrix(matrixInter);
      return 1;
    }

  } else {
    FloatMatrix *matrixComprasT = getFloatMatrix(matrixCompras->colunas, matrixCompras->linhas);
    if (!matrixComprasT) {
      perror("processMatrices");
      freeFloatMatrix(matrixCompras);
      freeFloatMatrix(matrixInter);
      return 1;
    }

    if (transposeMatrix(matrixComprasT, matrixCompras)) {
      fprintf(stderr, "processMatrices: transposeMatrix failed.\n");
      freeFloatMatrix(matrixCompras);
      freeFloatMatrix(matrixInter);
      freeFloatMatrix(matrixComprasT);
      return 1;
    }

    if (matrixMultiply(matrixInter, matrixCompras, matrixComprasT)) {
      fprintf(stderr, "processMatrices: matrixMultiply failed.\n");
      freeFloatMatrix(matrixCompras);
      freeFloatMatrix(matrixInter);
      freeFloatMatrix(matrixComprasT);
      return 1;
    }

    freeFloatMatrix(matrixComprasT);
  }

  freeFloatMatrix(matrixCompras);

  if (getMatrixSim(matrixSim, matrixInter)) {
    fprintf(stderr, "processMatrices: getMatrixSim failed.\n");
    freeFloatMatrix(matrixInter);
    return 1;
  }

  freeFloatMatrix(matrixInter);

  return 0;
}
