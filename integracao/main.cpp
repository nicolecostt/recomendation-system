#include "read.hpp"
#include "matrix.hpp"
#include "recommend.hpp"
#include <stdio.h>
#include <string.h>
#include <new>
#include <ctime>

int entregavel1() {
  char pathArquivo[] = "data/dados_venda_cluster_0.csv";

  Historico *historico = new (std::nothrow) Historico;
  if (!historico) {
    perror("historico");
    return 1;
  }

  if (getHistorico(historico, pathArquivo, -1)) {
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
  int fast = 0;

  Historico *historico = new Historico;
  if (!historico) {
    perror("entregavel2");
    return 1;
  }

  if (getHistorico(historico, pathArquivo, -1)) {
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

  if (processMatrices(matrixSim, historico, fast)) {
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
  int fast = 0;

  Historico *historico = new Historico;
  if (!historico) {
    perror("entregavel2");
    return 1;
  }

  if (getHistorico(historico, pathArquivo, -1)) {
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

  if (processMatrices(matrixSim, historico, fast)) {
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

int entregavel4() {
  char pathArquivo[] = "data/dados_venda_cluster_0.csv";
  int maxClientes;

  printf("Digite o tamanho da matriz (-1 para ler tudo):\n-> ");
  scanf("%d", &maxClientes);

  Historico *historico = new Historico;
  if (!historico) {
    perror("entregavel2");
    return 1;
  }

  if (getHistorico(historico, pathArquivo, maxClientes)) {
    delete historico;
    fprintf(stderr, "entregavel2: getHistorico failed.\n");
    return 1;
  }

  FloatMatrix *matrixSim0 = getFloatMatrix(historico->clientes.size(), historico->clientes.size());
  if (!matrixSim0) {
    perror("entregavel2");
    delete historico;
    return 1;
  }

  FloatMatrix *matrixSim1 = getFloatMatrix(historico->clientes.size(), historico->clientes.size());
  if (!matrixSim1) {
    perror("entregavel2");
    delete historico;
    return 1;
  }

  clock_t t1 = clock();

  if (processMatrices(matrixSim0, historico, 0)) {
    delete historico;
    freeFloatMatrix(matrixSim0);
    fprintf(stderr, "entregavel2: processMatrices failed.\n");
    return 1;
  }

  clock_t t2 = clock();

  if (processMatrices(matrixSim1, historico, 1)) {
    delete historico;
    freeFloatMatrix(matrixSim1);
    fprintf(stderr, "entregavel2: processMatrices failed.\n");
    return 1;
  }

  clock_t t3 = clock();

  double slowTime = (double)(t2 - t1) / CLOCKS_PER_SEC;
  double fastTime = (double)(t3 - t2) / CLOCKS_PER_SEC;

  printf("\n");
  printf("Tempo padrao:    %lf\n", slowTime);
  printf("Tempo adaptado:  %lf\n", fastTime);

  delete historico;
  freeFloatMatrix(matrixSim0);
  freeFloatMatrix(matrixSim1);

  return 0;
}

int main() {
  int entregavel;
  printf("Digite qual entregavel executar (1 - 4):\n-> ");
  scanf("%d", &entregavel);
  printf("\n");

  switch (entregavel) {
    case 1:
      if (entregavel1()) {
        fprintf(stderr, "main: entregavel1 Failed.\n");
        return 1;
      }
      break;

    case 2:
      if (entregavel2()) {
        fprintf(stderr, "main: entregavel2 failed.\n");
        return 1;
      }
      break;

    case 3:
      if (entregavel3()) {
        fprintf(stderr, "main: entregavel3 failed.\n");
        return 1;
      }
      break;

    case 4:
      if (entregavel4()) {
        fprintf(stderr, "main: entregavel4 failed.\n");
        return 1;
      }
      break;
  }

  return 0;
}
