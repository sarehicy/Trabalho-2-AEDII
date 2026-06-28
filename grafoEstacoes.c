#include <stdlib.h>
#include <string.h>

#include "grafoEstacoes.h"
#include "structs.h"

grafoEstacoes *criarGrafoEstacoes(){
    grafoEstacoes *grafoEst = malloc(sizeof(grafoEstacoes));
    // verificar grafoEstacoes

    grafoEst->vertices = malloc(sizeof(verticeEstacoes*));  // vetor de vertices
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

    // inserindo vertice no grafo respeitando sua ordenação
    insertionSort(grafo, vertice);

    int newQtd = grafo->qtdVertices +1;
    grafo->qtdVertices = newQtd;

    // Ajustando novo tamanho do vetor de vertices do grafo
    verticeEstacoes **aux = realloc(grafo->vertices, sizeof(verticeEstacoes*)*(newQtd+1));
    if (!aux) exit(0);
    grafo->vertices = aux;
}

void insertionSortVertice(grafoEstacoes *grafo, verticeEstacoes *vertice){

    // Insere vertice no final do vetor
    int qtd = grafo->qtdVertices;
    grafo->vertices[qtd] = vertice;

    // Insertion sort para inseri-lo na posição correta
    verticeEstacoes **vertices = grafo->vertices;
    int comp; verticeEstacoes *aux;

    int i = (qtd-1);
    while (i>=0){
        
        aux = vertices[i];
        comp = strcmp(vertice->nomeEstacao, aux->nomeEstacao);

        // vertice está na posição correta
        if (comp > 0) break;

        vertices[i+1] = vertices[i];
        vertices[i] = vertice;

        i-=1;
    }

}

