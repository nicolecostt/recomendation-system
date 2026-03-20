#include "read.hpp"
#include <stdio.h>

int getHistorico(Historico *historico, char *path_arquivo) {
  FILE *arquivo = NULL;
  Compra compra;
  arquivo = fopen(path_arquivo, "r");
  if (!arquivo) {
    perror("fopen");
    return 1;
  }

  int clienteIndice;
  int produtoIndice;

  char cabecalho[176];

  fgets(cabecalho, sizeof(cabecalho), arquivo);

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
    
    int contains = 0;
    for (int i: historico->listaCompras[clienteIndice]) {
      if (i == produtoIndice) {
        contains = 1;
        break;
      }
    }
    if (contains) {
      continue;
    }

    historico->listaCompras[clienteIndice].push_back(produtoIndice);
  }

  fclose(arquivo);
  arquivo = NULL;

  return 0;
}
