#include "read.hpp"
#include "matrix.hpp"
#include "recommend.hpp"
#include <stdio.h>
#include <string.h>
#include <new>

int entregavel1() {
  char pathArquivo[] = "data/dados_venda_cluster_0.csv";

  Historico *historico = new (std::nothrow) Historico;
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

  char cliente[16];
  int idCliente;
  int idClosest;
  while (strcmp(cliente, "-1") != 0) {
    printf("Digite o codigo do cliente (-1 para sair):\n-> ");
    scanf(" %15[^\n]", cliente);
    printf("\n");

    if (strcmp(cliente, "-1") == 0) {
      break;
    }

    if (historico->mapClientes.find(cliente) == historico->mapClientes.end()) {
      printf("Cliente nao encontrado!\n\n");
      continue;
    }

    idCliente = historico->mapClientes[cliente];
    
    idClosest = idCliente == 0 ? 1 : 0;
    for (int i = 0; i < historico->clientes.size(); i++) {
      if (i == idCliente) {
        continue;
      }
      if (matrixSim->elements[idCliente][i] < matrixSim->elements[idCliente][idClosest]) {
        idClosest = i;
      }
    }

    printf("Cliente mais proximo: %s\n\n", historico->clientes[idClosest].c_str());

    printf("Lista do cliente %s:\n", cliente);
    for (int valor: historico->listaCompras[idCliente]) {
      printf("%s\n", historico->produtos[valor].c_str());
    }
    printf("\nLista do cliente %s:\n", historico->clientes[idClosest].c_str());
    for (int valor: historico->listaCompras[idClosest]) {
      printf("%s\n", historico->produtos[valor].c_str());
    }
    printf("\n");

    printf("Distancia de %s para %s: %f\n\n",
        cliente,
        historico->clientes[idClosest].c_str(),
        matrixSim->elements[idCliente][idClosest]);
  }


  freeFloatMatrix(matrixSim);
  delete historico;
  return 0;
}

int entregavel3() {
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

  int k;
  char cliente[64];
  int idCliente;
  std::vector<Produto> *recomendados = new (std::nothrow) std::vector<Produto>;
  if (!recomendados) {
    freeFloatMatrix(matrixSim);
    delete historico;
    return 1;
  }
  
  printf("Digite quantos produtos recomendar por cliente:\n-> ");
  scanf("%d", &k);
  printf("\n");
  
  for (int i = 0; i < 3; i++) {
    printf("Digite o codigo do cliente %d:\n-> ", i + 1);
    scanf(" %63[^\n]", cliente);
    printf("\n");
  
    idCliente = historico->mapClientes[cliente];
    processRecommend(recomendados, idCliente, matrixSim, historico, k);
  
    printf("Produtos recomendados:\n");
    for (int j = 0; j < (int)recomendados->size(); j++) {
      printf("%s\n", historico->produtos[(*recomendados)[j].idProduto].c_str());
    }
    printf("\n");
  
    delete recomendados;
    recomendados = new (std::nothrow) std::vector<Produto>;
    if (!recomendados) {
      freeFloatMatrix(matrixSim);
      delete historico;
      return 1;
    }
  }
  
  delete recomendados;

  freeFloatMatrix(matrixSim);
  delete historico;
  return 0;
}

int main() {
  // if (entregavel1()) {
  //   fprintf(stderr, "main: entregavel1 Failed.\n");
  //   return 1;
  // }

  // if (entregavel2()) {
  //   fprintf(stderr, "main: entregavel2 failed.\n");
  //   return 1;
  // }

  if (entregavel3()) {
    fprintf(stderr, "main: entregavel2 failed.\n");
    return 1;
  }

  return 0;
}
