# Recommendation System

Sistema de recomendação de produtos desenvolvido em C/C++ como atividade prática da disciplina.  
O projeto é dividido em módulos responsáveis por:

- leitura e organização do histórico de compras;
- cálculo da similaridade entre clientes;
- geração de recomendações de produtos.

---

## Estrutura do Projeto

```bash
recommendation-system/
│
├── data/
│   └── dados_venda_cluster_0.csv
│
├── main.cpp
├── read.hpp
├── read.cpp
├── matrix.hpp
├── matrix.cpp
├── recommend.hpp
├── recommend.cpp
└── README.md