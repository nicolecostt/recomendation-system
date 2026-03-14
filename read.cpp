#include "read.h"
#include <stdio.h>
#include <stdlib.h>

int getHistorico(char *path_arquivo, Historico *historico) {
  FILE *arquivo;
  Compra compra;
  arquivo = fopen(path_arquivo, "r");
  if (arquivo == NULL) {
    perror("Erro ao abrir o arquivo.");
    return 1;
  }

  int clienteIndice;
  int produtoIndice;

  while (fscanf(arquivo, "%15[^,],%15[^,],%15[^,],%127[^\n]\n",
        compra.data,
        compra.codCliente,
        compra.codProduto,
        compra.nomeProduto) == 4) {

    if (historico->mapClientes.find(compra.codCliente) == historico->mapClientes.end()) {
      clienteIndice = historico->clientes.size();
      historico->clientes.push_back(compra.codCliente);
      historico->mapClientes[compra.codCliente] = clienteIndice;
      historico->listaCompras.push_back(std::list<int>());
    } else {
      clienteIndice = historico->mapClientes[compra.codCliente];
    }

    if (historico->mapProdutos.find(compra.codProduto) == historico->mapProdutos.end()) {
      produtoIndice = historico->produtos.size();
      historico->produtos.push_back(compra.nomeProduto);
      historico->mapProdutos[compra.codProduto] = produtoIndice;
    } else {
      produtoIndice = historico->mapProdutos[compra.codProduto];
    }
    
    historico->listaCompras[clienteIndice].push_back(produtoIndice);
  }

  fclose(arquivo);

  return 0;
}
