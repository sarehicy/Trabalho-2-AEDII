#ifndef GRAFOESTACOES_H
#define GRAFOSESTACOES_H

#include "structs.h"

void verificarGrafo(grafoEstacoes *grafo);

// Cria um grafo 
grafoEstacoes *criarGrafoEstacoes();

// Função que cria um vértice e insere no grafo
void criarVertice(grafoEstacoes *grafo, char *nomeEstacao, int tamNomeEstacao);

// Insere um vértice no grafo ordenando-o
void insertionSortVertice(grafoEstacoes *grafo, verticeEstacoes *vertice);

void criarAresta();

void insertionSortAresta();


#endif