#include <vector>
#include <map>
#include <list>

typedef struct {
  std::vector<std::string> clientes;
  std::map<std::string,int> mapClientes;
  std::vector<std::string> produtos;
  std::map<std::string,int> mapProdutos;
  std::vector<std::list<int>> listaCompras;
} Historico;

typedef struct {
  char data[16];
  char codCliente[16];
  char codProduto[16];
  char nomeProduto[128];
} Compra;

int getHistorico(char *path_arquivo, Historico *historico);
