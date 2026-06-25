#ifndef CABECALHOS_H
#define CABECALHOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares.h"
#include "cabecalhos.h"
#include "arquivos.h"
#include "funcionalidades.h"
#include "registros.h"
#include "structs.h"

// Indicadores da consistência do arquivo
#define consistente '1'        
#define inconsistente '0'

#define tamCabecalho 17

#define add 1       // Indica inserção de registro no fim do arquivo
#define insert 2     // Indica insersão de registro no meio do arquivo
#define rmv 0       // Indica remoção de registro

// Verifica integridade do ponteiro do cabeçalho
void verificarHeader(header *cabecalho);

// Verifica integridade do ponteiro do cabeçalho do arquivo índice
void verificarHeaderIndex(headerIndex *cabecalho);

/* Cria uma struct header.
   Inicializa a struct com valores default e retorna seu ponteiro */
header *criarHeader();

/* Atualiza campos do cabeçalho do arquivo bin toda vez que uma operação é realizada
   Recebe cabeçalho, tabela auxiliar, registro removido ou inserido e operação realizada*/
void atualizarHeader(header *header, dadosHeader *dados, reg *registro, int op);


/* Insere novo par no vetor pares e incrementa quantidade de pares*/
void inserirPar(header *cabecalho, dadosHeader *dados, int codEst, int codProx);

/* Remove par do reg removido do vetor pares e decrementa quantidade de pares*/
void removerPar(header *cabecalho, dadosHeader *dados, int codEst, int codProx);


/* Cria e inicializa dadosHeader */
dadosHeader *criarDadosHeader();

/*Dado um binário, monta tabela auxiliar*/
void montarDadosHeader(FILE *arqBin, reg *registro, dadosHeader *dados);


/* Libera memória alocada para struct dadosHeader */
void liberarDadosHeader(dadosHeader *dados);

#endif