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
    aresta->qtdLinhas = 1;
    aresta->nomesLinhas = malloc(sizeof(char*)*(aresta->qtdLinhas + 1));
    
    tamNome = strlen(nomeLinha);
    aresta->nomesLinhas[0] = malloc(sizeof(char*));
    strcpy(aresta->nomesLinhas[0], nomeLinha);
    

    aresta->distProxEstacao = distancia;

    aresta->next = NULL;

    return aresta;
}


void adicionarNomeLinha(arestaEstacoes *aresta, char *nomeLinha){

    // Verifica se essa linha já está cadastrada
    for(int i = 0; i < aresta->qtdLinhas; i++){
        if(strcmp(aresta->nomesLinhas[i], nomeLinha) == 0){
            return; // Já existe, não adiciona novamente
        }
    }

    // Aumenta a quantidade de linhas
    aresta->qtdLinhas++;

    // Redimensiona o vetor de ponteiros para caber a nova linha
    char **aux = realloc(aresta->nomesLinhas, sizeof(char*) * aresta->qtdLinhas);
    if(aux == NULL){
        exit(0);
    }
    aresta->nomesLinhas = aux;

    // Aloca memória para a nova string em um ponteiro temporário
    int tamNome = strlen(nomeLinha);
    char *novaLinha = malloc(sizeof(char) * (tamNome + 1));
    strcpy(novaLinha, nomeLinha);

    // Insere o nome no lugar certo alfabéticamente
    // Começa do último elemento válido antes do realloc
    int i = aresta->qtdLinhas - 2; 
    
    while (i >= 0 && strcmp(aresta->nomesLinhas[i], novaLinha) > 0) {
        aresta->nomesLinhas[i + 1] = aresta->nomesLinhas[i];
        i--;
    }
    
    // Insere o ponteiro da nova linha na posição alfabética correta
    aresta->nomesLinhas[i + 1] = novaLinha;
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
        // A aresta já existe. Apenas adiciona o novo nome de linha
        adicionarNomeLinha(aux, aresta->nomesLinhas[0]);

        // A aresta criada não será utilizada
        free(aresta->nomesLinhas[0]);
        free(aresta->nomesLinhas);
        free(aresta->nomeEstacao);
        free(aresta);

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
            // A aresta já existe. Apenas adiciona o novo nome de linha
            adicionarNomeLinha(aux->next, aresta->nomesLinhas[0]);

            // A aresta criada não será utilizada
            free(aresta->nomesLinhas[0]);
            free(aresta->nomesLinhas);
            free(aresta->nomeEstacao);
            free(aresta);

            return;
        }
        // Percorre lista encadeada
        aux = aux->next;
    }
    /*  #   Insersão no Final   #   */
    aux->next = aresta;
}



void imprimirGrafo(grafoEstacoes *grafo) {
    for (int i = 0; i < grafo->qtdVertices; i++) {
        verticeEstacoes *vertice = grafo->vertices[i];

        // Se o vértice não tem nenhuma aresta, não imprime e passa para o próximo
        if (vertice->arestas == NULL) {
            continue;
        }

        // Imprime o nome da estação de origem
        printf("%s", vertice->nomeEstacao);

        // Percorre a lista de arestas
        arestaEstacoes *arestaAux = vertice->arestas;
        while (arestaAux != NULL) {
            
            // Imprime a próxima estação e a distância com vírgulas
            printf(", %s, %d", arestaAux->nomeEstacao, arestaAux->distProxEstacao);

            // Imprime todas as linhas dessa aresta
            for (int j = 0; j < arestaAux->qtdLinhas; j++) {
                printf(", %s", arestaAux->nomesLinhas[j]);
            }

            // Vai para a próxima aresta
            arestaAux = arestaAux->next;
        }

        // Quando finalizar o vértice, quebra a linha
        printf("\n");
    }
}