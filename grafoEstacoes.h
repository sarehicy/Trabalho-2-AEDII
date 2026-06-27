#ifndef GRAFOESTACOES_H
#define GRAFOSESTACOES_H

#include "structs.h"

// Cria um grafo 
grafoEstacoes *criarGrafoEstacoes();

// Função que cria um vértice e insere no grafo
void criarVertice(grafoEstacoes *grafo, char *nomeEstacao, int tamNomeEstacao);



#endif