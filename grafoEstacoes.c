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
    insertionSortVertice(grafo, vertice);

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

int buscarVertice(grafoEstacoes *grafo, char *nomeEstacao){
    int comp; verticeEstacoes *aux;

    int esq = 0;
    int dir = (grafo->qtdVertices) -1;
    int meio;
    
    // Busca binária
    while (esq <= dir){
        meio = (esq + dir)/2;

        aux = grafo->vertices[meio];
        comp = strcmp(nomeEstacao, aux->nomeEstacao);

        if (comp == 0){
            return meio;
        }
        else if(comp < 0){
            dir = meio -1;
        }
        else{
            esq = meio +1;
        }
    }

    // Não encontrou o vertice
    return -1;
}

arestaEstacoes *criarAresta(char *nomeEstacao, int distancia, char *nomeLinha){
    arestaEstacoes *aresta = malloc(sizeof(arestaEstacoes));
    // verificar aresta
    
    int tamNome;

    // Preenchendo campo "Nome Estação"
    tamNome = strlen(nomeEstacao);
    aresta->nomeEstacao = malloc(sizeof(char)*(tamNome + 1));
    strcpy(aresta->nomeEstacao, nomeEstacao);

    // Criando vetor de nomes linha
    // aresta->qtdLinhas = 1;
    // aresta->nomesLinhas = malloc(sizeof(char*)*(aresta->qtdLinhas + 1));
    
    // tamNome = srtlen(nomeLinha);
    // aresta->nomesLinhas[0] = malloc(sizeof(char)*(tamNome +1));
    // strcpy(aresta->nomesLinhas[0], nomeLinha);
    

    aresta->distProxEstacao = distancia;

    aresta->next = NULL;

    return aresta;
}

void insertionSortAresta(verticeEstacoes *vertice, arestaEstacoes *aresta){

    int comp;

    arestaEstacoes *aux = vertice->arestas;

    /*  # Insersão da primeira aresta   #   */
    if(aux == NULL){
        vertice->arestas = aresta;

        return;
    }

    /*  #   Insersão no Ínicio   #   */
    comp = strcmp(aresta->nomeEstacao, aux->nomeEstacao);

    // Se a nova aresta precisa ser inserida no começo
    if(comp < 0){
        vertice->arestas = aresta;
        aresta->next = aux;
        
        return;
    }
    if (comp == 0){ 
        // Nomes de estação iguais, desempata por nome linha

        return;
    }


    /*  #   Insersão no Meio    #   */
    while (aux->next != NULL){

        comp = strcmp(aresta->nomeEstacao, aux->next->nomeEstacao);

        // Se a nova aresta precisa vir antes que a próxima
        if (comp < 0){ 
            aresta->next = aux->next;
            aux->next = aresta;

            return;
        }
        if (comp == 0){
            // Nomes de estação iguais, desempata por nome linha

            return;
        }


        // Percorre lista encadeada
        aux = aux->next;
    }

    /*  #   Insersão no Final   #   */
    aux->next = aresta;




}