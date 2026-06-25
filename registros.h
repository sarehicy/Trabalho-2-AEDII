#ifndef REGISTROS_H
#define REGISTROS_H

#include "auxiliares.h"
#include "cabecalhos.h"
#include "arquivos.h"
#include "funcionalidades.h"
#include "registros.h"
#include "structs.h"

#define bytesFixos 37       // Soma da quantidade de bytes dos campos fixos do registro
#define tamRegistro 80      // Tamanho em bytes do registro

/* Verifica ponteiro da struct registro */
void verificarReg(reg *registro);

void verificarRegIndex(regIndex *registro);

/* Inicializa registro de dados com valores default */
void inicializarRegistro(reg *registro);

/* Imprime campos do registro de dados na saída padrão */
void imprimirRegistro(reg *registro);

/*Dado um Registro, compara todos os campos buscados.
   Retorna 1 se o registro corresponder a busc, 0 se não.*/
int buscaRegistro(reg *registro, char **linha, int qtdCampos);

/* Compara o valor de um campo do registro de dados com o valor do campo buscado.
    Retorna 1 se os campos forem iguais, 0 se forem diferentes.*/
int buscaCampo(reg *registro, char campo[], char valor[]);

/*Atualiza os campos de um registro.
Os campos e os valores novos são passados pelo vetor linha.*/
void atualizarRegistro(reg *registro, char **linha, int qtdAtualizacoes);

/*Monta registro com uma linha do terminal*/
void montarRegistroCmd(reg *registro);

#endif