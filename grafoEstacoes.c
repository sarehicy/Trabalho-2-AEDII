#include <stdlib.h>
#include <string.h>

#include "grafoEstacoes.h"
#include "structs.h"

grafoEstacoes *criarGrafoEstacoes(){
    grafoEstacoes *grafoEst = malloc(sizeof(grafoEstacoes));
    // verificar grafoEstacoes

    grafoEst->vertices = malloc(sizeof(verticeEstacoes*));
    grafoEst->qtdVertices = 0;

    return grafoEst;
}

void criarVertice(grafoEstacoes *grafo, char *nomeEstacao, int tamNomeEstacao){
    verticeEstacoes *vertice = malloc(sizeof(vertice));
    // verificar vertice

    // alocando vetor para o nome da estação
    vertice->nomeEstacao = malloc(sizeof(char)*(tamNomeEstacao+1));
    strcpy(vertice->nomeEstacao, nomeEstacao);

    vertice->arestas = NULL;

    // inserindo vertice no grafo
    int pos = grafo->qtdVertices;
    grafo->vertices[pos] = vertice;
    pos+=1;
    grafo->qtdVertices = pos;


    // Ajustando novo tamanho do vetor de vertices do grafo
    verticeEstacoes **aux = realloc(grafo->vertices, sizeof(verticeEstacoes*)*(pos+1));
    if (!aux) exit(0);
    grafo->vertices = aux;
}

