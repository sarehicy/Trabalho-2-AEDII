#include "funcionalidades.h"

void funcDez(){
    /*      # structs #     */
    reg *registro = malloc(sizeof(reg));
    verificarReg(registro);

    header *cabecalho = malloc(sizeof(header));
    verificarHeader(cabecalho);

    grafoEstacoes *grafo = criarGrafoEstacoes();
    //verificarGrafo(grafo);

    /*      # User Input #      */
    char inBin[100];
    scanf("%s", inBin);

    /* # Abertura e verificação de arquivos # */ 
    FILE *arqBin = fopen(inBin, "rb+");
    verificarArq(arqBin);

    lerCabecalho(arqBin, cabecalho);

    // Percorre arquivo criando vértices
    while(check_eof(arqBin)){

        inicializarRegistro(registro);
        lerRegistro(arqBin, registro);

        // Só cria novo vértice se já não existir um vertice com o mesmo nome estação
        if (buscarVertice(grafo, registro->nomeEstacao) == -1){
            criarVertice(grafo, registro->nomeEstacao, registro->tamNomeEstacao);
        }
    }

    // Testando :P
    for(int i = 0; i<grafo->qtdVertices; i++){
        printf("%s\n", grafo->vertices[i]->nomeEstacao);
    }

    /*   #   Fechando Arquivos e BinarioNaTela()    # */

    free(registro);
    free(cabecalho);
    fclose(arqBin);
}