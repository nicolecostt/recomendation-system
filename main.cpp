#include "read.hpp"
#include "matrix.hpp"
#include <stdio.h>
#include <string.h>

int entregavel1() {
  char pathArquivo[] = "data/dados_venda_cluster_0.csv";

  Historico *historico = new Historico;
  if (!historico) {
    perror("historico");
    return 1;
  }

  if (getHistorico(historico, pathArquivo)) {
    fprintf(stderr, "entregavel1: getHistorico failed.\n");
    delete historico;
    return 1;
  }
  
  char cliente[16];
  printf("Digite o codigo do cliente (-1 para sair):\n-> ");
  scanf(" %15[^\n]", cliente);
  while (strcmp(cliente, "-1") != 0) {
    if (historico->mapClientes.find(cliente) == historico->mapClientes.end()) {
      printf("Cliente nao encontrado!\n\n");
      printf("Digite o codigo do cliente (-1 para sair):\n-> ");
      scanf(" %15[^\n]", cliente);
      continue;
    }
    for (int valor: historico->listaCompras[historico->mapClientes[cliente]]) {
      printf("%s\n", historico->produtos[valor].c_str());
    }
    printf("\n");
    printf("Digite o codigo do cliente (-1 para sair):\n-> ");
    scanf(" %15[^\n]", cliente);
  }

  delete historico;
  return 0;
}

int entregavel2() {
  char pathArquivo[] = "data/dados_venda_cluster_0.csv";

  Historico *historico = new Historico;
  if (!historico) {
    perror("entregavel2");
    return 1;
  }

  if (getHistorico(historico, pathArquivo)) {
    delete historico;
    fprintf(stderr, "entregavel2: getHistorico failed.\n");
    return 1;
  }

  FloatMatrix *matrixSim = getFloatMatrix(historico->clientes.size(), historico->clientes.size());
  if (!matrixSim) {
    perror("entregavel2");
    delete historico;
    return 1;
  }

  if (processMatrices(matrixSim, historico)) {
    delete historico;
    freeFloatMatrix(matrixSim);
    fprintf(stderr, "entregavel2: processMatrices failed.\n");
    return 1;
  }

  char cliente1[16];
  char cliente2[16];
  int idCliente1, idCliente2;
  while (strcmp(cliente1, "-1") != 0) {
    printf("Digite o codigo do cliente 1 (-1 para sair):\n-> ");
    scanf(" %15[^\n]", cliente1);

    if (strcmp(cliente1, "-1") == 0) {
      break;
    }

    if (historico->mapClientes.find(cliente1) == historico->mapClientes.end()) {
      printf("Cliente nao encontrado!\n\n");
      continue;
    }

    printf("Digite o codigo do cliente 2 (-1 para voltar):\n-> ");
    scanf(" %15[^\n]", cliente2);

    while (historico->mapClientes.find(cliente2) == historico->mapClientes.end() &&
           strcmp(cliente2, "-1") != 0) {
      printf("Cliente nao encontrado!\n\n");
      printf("Digite o codigo do cliente 2 (-1 para voltar):\n-> ");
      scanf(" %15[^\n]", cliente2);
    }

    if (strcmp(cliente2, "-1") == 0) {
      printf("\n");
      continue;
    }

    idCliente1 = historico->mapClientes[cliente1];
    idCliente2 = historico->mapClientes[cliente2];

    printf("\nCompras do cliente 1:\n");
    for (int id: historico->listaCompras[idCliente1]) {
      printf("%s\n", historico->produtos[id].c_str());
    }
    printf("\nCompras do cliente 2:\n");
    for (int id: historico->listaCompras[idCliente2]) {
      printf("%s\n", historico->produtos[id].c_str());
    }
    printf("\nDistancia de Jaccard: %.3f\n\n", matrixSim->elements[idCliente1][idCliente2]);
  }

  freeFloatMatrix(matrixSim);
  delete historico;
  return 0;
}

int main() {
  // if (entregavel1()) {
  //   fprintf(stderr, "main: entregavel1 Failed.\n");
  //   return 1;
  // }

  if (entregavel2()) {
    fprintf(stderr, "main: entregavel2 failed.\n");
    return 1;
  }

  return 0;
}
