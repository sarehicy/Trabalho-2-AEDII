#include "funcionalidades.h"

void funcDez(){
    /*      # structs #     */
    reg *registro = malloc(sizeof(reg));
    verificarReg(registro);

    reg *registroAux = malloc(sizeof(reg));
    verificarReg(registroAux);

    header *cabecalho = malloc(sizeof(header));
    verificarHeader(cabecalho);

    grafoEstacoes *grafo = criarGrafoEstacoes();
    //verificarGrafo(grafo);

    arestaEstacoes *arestaAux;

    /*      # User Input #      */
    char inBin[100];
    scanf("%s", inBin);

    /* # Abertura e verificação de arquivos # */ 
    FILE *arqBin = fopen(inBin, "rb+");
    verificarArq(arqBin);

    lerCabecalho(arqBin, cabecalho);

    /*  #   Percorre arquivo Criando Vertices   #    */
    while(check_eof(arqBin)){

        inicializarRegistro(registro);
        if(lerRegistro(arqBin, registro)) continue;

        // Só cria novo vértice se não existir um vertice com o mesmo nome estação
        if (buscarVertice(grafo, registro->nomeEstacao) == -1){
            criarVertice(grafo, registro->nomeEstacao, registro->tamNomeEstacao);
        }

    }


    /*  #   Criando Arestas   #   */
    int verticeID;                          // Posição de uma vértice do grafo
    long pos;                                // Ponteiro que acompanha o laço que percorre o arquivo

    fseek(arqBin, tamCabecalho, SEEK_SET);
    while(check_eof(arqBin)){
        inicializarRegistro(registro);
        if(lerRegistro(arqBin, registro)) continue;

        /* Durante a criação dos vertices, iremos mover o ponteiro para encontrar
            o nome da próxima estação, mas o laço while precisa percorrer todo o arquivo
            por isso guardamos o valor da posição de onde ele deve prosseguir*/
        pos = ftell(arqBin); 

        // Busca estacao no grafo
        verticeID = buscarVertice(grafo, registro->nomeEstacao);
        if (verticeID != -1){
            /* Códigos estação a serem buscados */
            char codProxEst[10], codEstacaoInteg[10]; // Strings para busca
            snprintf(codProxEst, sizeof(codProxEst), "%d" , (registro->codProxEstacao));
            snprintf(codEstacaoInteg, sizeof(codEstacaoInteg), "%d" , (registro->codEstacaoInteg));


            /* Criando aresta para próxima estação*/
            if (strcmp(codProxEst, "-1")){

                // Move ponteiro para o ínicio do arquivo para começar a busca
                fseek(arqBin, tamCabecalho, SEEK_SET);
                while(check_eof(arqBin)){

                    inicializarRegistro(registroAux);
                    if (lerRegistro(arqBin, registroAux)) continue;

                    if (buscaCampo(registroAux, "codEstacao", codProxEst)){

                        arestaAux = criarAresta(registroAux->nomeEstacao, registro->distProxEstacao, registro->nomeLinha);
                        insertionSortAresta(grafo->vertices[verticeID], arestaAux);

                        break;
                    }
                }
            }

            /* Criando aresta para estação de integração */
            if (strcmp(codEstacaoInteg, "-1")){

                // Move ponteiro para o ínicio do arquivo para começar a busca
                fseek(arqBin, tamCabecalho, SEEK_SET);
                while(check_eof(arqBin)){

                    inicializarRegistro(registroAux);
                    if (lerRegistro(arqBin, registroAux)) continue;

                    if (buscaCampo(registroAux, "codEstacao", codEstacaoInteg)){

                        // Estação e estação de integração com o mesmo nome
                        if(!strcmp(registro->nomeEstacao, registroAux->nomeEstacao)) break;

                        arestaAux = criarAresta(registroAux->nomeEstacao, 0, "Integração");
                        insertionSortAresta(grafo->vertices[verticeID], arestaAux);

                        break;
                    }
                }
            }

        }

        fseek(arqBin, pos, SEEK_SET);

    }

    imprimirGrafo(grafo);

    // Testando :P
    /*
    for(int i = 0; i<grafo->qtdVertices; i++){
        printf("vertice: %s ", grafo->vertices[i]->nomeEstacao);
        arestaAux = grafo->vertices[i]->arestas;

        while(arestaAux != NULL){
            printf("aresta: %s ", arestaAux->nomeEstacao);

            for(int j=0; j<arestaAux->qtdLinhas; j++){
                printf("linha %s ", arestaAux->nomesLinhas[j]);
            }

            arestaAux = arestaAux->next;
        }

        printf("\n");
    }*/

    /*   #   Fechando Arquivos e BinarioNaTela()    # */

    free(registroAux);
    free(registro);
    free(cabecalho);
    fclose(arqBin);
}