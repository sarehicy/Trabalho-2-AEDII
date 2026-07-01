#ifndef GRAFOESTACOES_H
#define GRAFOESTACOES_H

#include <stdlib.h>
#include <string.h>
#include "auxiliares.h"
#include "cabecalhos.h"
#include "arquivos.h"
#include "funcionalidades.h"
#include "registros.h"
#include "structs.h"
#include "grafoEstacoes.h"

//void verificarGrafo(grafoEstacoes *grafo);

// Cria um grafo 
grafoEstacoes *criarGrafoEstacoes();

// Função que cria um vértice e insere no grafo
void criarVertice(grafoEstacoes *grafo, char *nomeEstacao, int tamNomeEstacao);

// Insere um vértice no grafo ordenando-o
void insertionSortVertice(grafoEstacoes *grafo, verticeEstacoes *vertice);

// Busca uma determinada estação no grafo e retorna sua posição
// Retorna -1 se não encontrar a aresta
int buscarVertice(grafoEstacoes *grafo, char *nomeEstacao);

arestaEstacoes *criarAresta(char *nomeEstacao, int distancia, char *nomeLinha);

void insertionSortAresta(verticeEstacoes *vertice, arestaEstacoes *aresta);


#endif