import csv
import pyrecommend

def carregar_historico(path, max_clientes=-1):
    historico = pyrecommend.Historico()

    with open(path, newline='', encoding='utf-8') as f:
        leitor = csv.reader(f)
        next(leitor)  # pula cabeçalho

        for linha in leitor:
            data, cod_cliente, cod_produto, nome_produto = linha

            if cod_cliente not in historico.mapClientes:
                if max_clientes != -1 and len(historico.clientes) >= max_clientes:
                    continue
                indice_cliente = len(historico.clientes)
                historico.clientes.append(cod_cliente)
                historico.mapClientes[cod_cliente] = indice_cliente
                historico.listaCompras.append([])
            else:
                indice_cliente = historico.mapClientes[cod_cliente]

            if cod_produto not in historico.mapProdutos:
                indice_produto = len(historico.produtos)
                historico.produtos.append(nome_produto)
                historico.mapProdutos[cod_produto] = indice_produto
            else:
                indice_produto = historico.mapProdutos[cod_produto]

            if indice_produto not in historico.listaCompras[indice_cliente]:
                historico.listaCompras[indice_cliente].append(indice_produto)

    return historico

def entregavel1():
    historico = carregar_historico("../data/dados_venda_cluster_0.csv")
    cliente = input("Digite o codigo do cliente (-1 para sair):\n-> ")
    while cliente != "-1":
        if cliente not in historico.mapClientes:
            print("Cliente nao encontrado!\n")
        else:
            id_cliente = historico.mapClientes[cliente]
            for id_produto in historico.listaCompras[id_cliente]:
                print(historico.produtos[id_produto])
            print()
        cliente = input("Digite o codigo do cliente (-1 para sair):\n-> ")

def entregavel2():
    historico = carregar_historico("../data/dados_venda_cluster_0.csv")
    matrix_sim = pyrecommend.calcular_similaridade(historico, 0)
    cliente = input("Digite o codigo do cliente (-1 para sair):\n-> ")
    while cliente != "-1":
        if cliente not in historico.mapClientes:
            print("Cliente nao encontrado!\n")
            cliente = input("Digite o codigo do cliente (-1 para sair):\n-> ")
            continue
        id_cliente = historico.mapClientes[cliente]
        id_closest = 1 if id_cliente == 0 else 0
        for i in range(len(historico.clientes)):
            if i == id_cliente:
                continue
            if matrix_sim[id_cliente][i] < matrix_sim[id_cliente][id_closest]:
                id_closest = i
        print(f"Cliente mais proximo: {historico.clientes[id_closest]}\n")
        print(f"Lista do cliente {cliente}:")
        for v in historico.listaCompras[id_cliente]:
            print(historico.produtos[v])
        print(f"\nLista do cliente {historico.clientes[id_closest]}:")
        for v in historico.listaCompras[id_closest]:
            print(historico.produtos[v])
        print(f"\nDistancia de {cliente} para {historico.clientes[id_closest]}: {matrix_sim[id_cliente][id_closest]}\n")
        cliente = input("Digite o codigo do cliente (-1 para sair):\n-> ")

def entregavel3():
    historico = carregar_historico("../data/dados_venda_cluster_0.csv")
    matrix_sim = pyrecommend.calcular_similaridade(historico, 0)
    k = int(input("Digite quantos produtos recomendar por cliente:\n-> "))
    print()
    for i in range(3):
        cliente = input(f"Digite o codigo do cliente {i + 1}:\n-> ")
        print()
        id_cliente = historico.mapClientes[cliente]
        recomendados = pyrecommend.recomendar_produtos(historico, matrix_sim, id_cliente, k)
        print("Produtos recomendados:")
        for id_produto, ranking in recomendados:
            print(historico.produtos[id_produto])
        print()

def entregavel4():
    max_clientes = int(input("Digite o tamanho da matriz (-1 para ler tudo):\n-> "))
    historico = carregar_historico("../data/dados_venda_cluster_0.csv", max_clientes)
    tempo_padrao, tempo_otimizado = pyrecommend.benchmark(historico)
    print(f"\nTempo padrao:    {tempo_padrao}")
    print(f"Tempo adaptado:  {tempo_otimizado}")

if __name__ == "__main__":
    entregavel = int(input("Digite qual entregavel executar (1 - 4):\n-> "))
    print()
    if entregavel == 1:
        entregavel1()
    elif entregavel == 2:
        entregavel2()
    elif entregavel == 3:
        entregavel3()
    elif entregavel == 4:
        entregavel4()
