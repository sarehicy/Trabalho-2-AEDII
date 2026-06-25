#ifndef STRUCTS_H
#define STRUCTS_H

#include "auxiliares.h"
#include "cabecalhos.h"
#include "arquivos.h"
#include "funcionalidades.h"
#include "registros.h"
#include "structs.h"

/* Registro de Cabeçalho */
typedef struct header {
    char status;            // 1 - Consistente | 0 - Inconsistente
    int topo;               // Byteoffset do último registro removido | -1 default
    int proxRRN;            // Próximo RRN disponível
    int totalEstacoes;      // Quantidade de estações diferentes 
    int totalPares;         // Quantidade de pares (codEstacao, codProxEstacao) diferentes
} header;


/* Tabela Auxiliar do Cabeçalho */
typedef struct dadosHeader {
    char **estacoes;            // Vetor com os diferetes nomes de estações
    int *estacoesOcorrencias;   // Quantos registros tem a mesma estação
    int qtdEstacoes;

    int (*pares)[2];        // Vetor dinâmico com os pares (codEstacao, ProxEstacao)
    int qtdPares;
} dadosHeader;

/* Registro de dados */
typedef struct reg {
    char rem;               // 1 - Removido | 0 - Não Removido
    int prox;               //RRN do prox elemento removido | -1 - Default

    int codEstacao;         
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaInteg;
    int codEstacaoInteg;
    
    int tamNomeEstacao;     // Indica o tamanho da string de tamanho variavel "nomeEstacao"
    char nomeEstacao[100];
    int tamNomeLinha;       // Indica o tamanho da string de tamanho variavel "nomeLinha"
    char nomeLinha[100];
} reg;

/* Cabeçalho do arquivo Indexa Estação */
typedef struct headerIndex{
    char status;
    
} headerIndex;

/* Registro de dados do arquivo Indexa Estação*/
typedef struct regIndex{
    int codEstacao;
    int RRN;
} regIndex;



#endif