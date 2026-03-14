#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

typedef struct {
  vector<string> cliente;
  map<string,int> mapCliente;
  vector<string> produto;
  map<string,int> mapProduto;
  vector<list<int>> listaCompras;
} Historico;

typedef struct {
  char data[16];
  char codCliente[16];
  char codProduto[16];
  char nomeProduto[128];
} Compra;

Historico *getHistorico(char *path_arquivo) {
  FILE *arquivo;
  Compra compra;
  arquivo = fopen(path_arquivo, "r");
  if (arquivo == NULL) {
    perror("Erro ao abrir o arquivo.");
    return NULL;
  }

  while (fscanf(arquivo, "%15[^,],%15[^,],%15[^,],%128[^\n]\n",
        compra.data,
        compra.codCliente,
        compra.codProduto,
        compra.nomeProduto) == 4) {
    
  }
}
