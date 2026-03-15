#include "read.hpp"
#include <stdio.h>
#include <string.h>

int entregavel1() {
  Historico *historico = new Historico;
  char pathArquivo[] = "data/dados_venda_cluster_0.csv";

  if (getHistorico(historico, pathArquivo)) {
    perror("Erro ao ler o csv");
  }
  
  char cliente[16];
  printf("Digite o codigo do cliente (-1 para sair):\n-> ");
  scanf("%15s", cliente);
  while (strcmp(cliente, "-1") != 0) {
    if (historico->mapClientes.find(cliente) == historico->mapClientes.end()) {
      printf("Cliente nao encontrado!\n\n");
      printf("Digite o codigo do cliente (-1 para sair):\n-> ");
      scanf("%15s", cliente);
      continue;
    }
    for (int valor: historico->listaCompras[historico->mapClientes[cliente]]) {
      printf("%s\n", historico->produtos[valor].c_str());
    }
    printf("\n");
    printf("Digite o codigo do cliente (-1 para sair):\n-> ");
    scanf("%15s", cliente);
  }

  delete historico;

  return 0;
}

int main() {
  entregavel1();
  return 0;
}
