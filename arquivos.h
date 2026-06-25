#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stdio.h>
#include "auxiliares.h"
#include "cabecalhos.h"
#include "funcionalidades.h"
#include "registros.h"
#include "structs.h"

/* Escreve registro (struct) no arquivo binário */
void escreverRegistro(FILE *arq, reg *registro);

/* Escreve o cabeçalho no arquivo binário   */
void escreverHeader(FILE *arq, header *cabecalho);

/*Escreve cabeçalho no arquivo índice*/
void escreverHeaderIndex(FILE *arq, headerIndex *cabecalho);

/*Escreve registro no arquivo índice*/
void escreverRegIndex(FILE *arqIndex, regIndex *registroIndex);

/* Verifica ponteiro do arquivo */
void verificarArq(FILE *arq);

/* Lê um registro do arquivo .csv e monta um registro na memória (struct)*/
void montarRegistro(FILE *arq, reg *registro);

/* Lê registro do arquivo .bin e monta um registro na memória (struct).
 Retorna 0 se a leitura for bem sucedida, retorna 1 se não conseguir ler*/
int lerRegistro(FILE *arq, reg *registro);

/* Lê cabeçalho do arquivo binario e verifica consistência do arquivo*/
void lerCabecalho(FILE *arq, header *cabecalho);

/* Lê cabeçalho do arquivo binario índice e verifica consistência do arquivo*/
void lerCabecalhoIndex(FILE *arq, headerIndex *cabecalho);

/*lê registro do arquivo índice*/
void lerRegistroIndex(FILE *arqBinIndex, regIndex *registroIndex);

/*Reordena o arquivo index para manter suas propriedades*/
void reordenarArqIndex(FILE *arqIndex);

#endif