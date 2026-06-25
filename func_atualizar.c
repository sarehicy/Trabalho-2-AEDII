#include "funcionalidades.h"


void funcNove(dadosHeader *dados){
    /*      # structs #     */
    reg *registro = malloc(sizeof(reg));
    verificarReg(registro);

    header *cabecalho = malloc(sizeof(header));
    verificarHeader(cabecalho);

    headerIndex *cabecalhoIndex = malloc(sizeof(headerIndex));
    verificarHeaderIndex(cabecalhoIndex);


    /*      # User Input #      */
    char inBin[100];
    char inBinIndex[100];
    int n;
    scanf("%s", inBin);
    scanf("%s", inBinIndex);
    scanf("%d", &n);


    // # Abertura e verificação de arquivos # // 
    FILE *arqBin = fopen(inBin, "rb+");
    verificarArq(arqBin);

    FILE *arqBinIndex = fopen(inBinIndex, "rb+");
    verificarArq(arqBinIndex);

    lerCabecalhoIndex(arqBinIndex, cabecalhoIndex);
    lerCabecalho(arqBin, cabecalho);

    montarDadosHeader(arqBin, registro, dados);
    dados->qtdPares = cabecalho->totalPares;
    /* ATENÇÃO!
        A funcão montarDadosHeader está contando os pares corretamente, no entanto no caso de teste 7
        a quantidade de pares do cabeçalho do arquivo está incorreta, o que acabava provocando erro no resultado final.
        Para evitar problemas a quantidade de pares da struct auxiliar foi igualada a quantidade de pares do cabeçalho. :)
        Conversado com o monitor Renan Catarin.
    */

    /*      # Buscas #       */
    cabecalho->status = inconsistente; // porque vai escrever no arq
    escreverHeader(arqBin, cabecalho);
    cabecalhoIndex->status = inconsistente;
    escreverHeaderIndex(arqBinIndex, cabecalhoIndex);

    int qtdCampos, qtdAtualizacoes;
    while(n--){
        scanf("%d", &qtdCampos);
        char *linhaBusca[qtdCampos*2];           // Vetor com os *pares "campo" e "valor"  sendo buscados
        montarBusca(linhaBusca, qtdCampos);

        scanf("%d", &qtdAtualizacoes);
        char *linhaAtualizacoes[qtdAtualizacoes*2];     // Vetor com os *pares "campo" e "valor" que serão atualizados
        montarBusca(linhaAtualizacoes, qtdAtualizacoes);


        /*  #   Busca Indexada    #   */
        int codEst; // Guarda o valor do codEstacao do registro antes da sua atualização
        int RRN = -2;
        for(int i = 0; i<(qtdCampos*2); i+=2){
            if (!(strcmp(linhaBusca[i], "codEstacao"))){     // Se algum campo for codEstacao faz a busca indexada
                RRN = buscaIndexada(arqBinIndex, atoi(linhaBusca[i+1]));

                if (RRN != -1){  // Encontrou registro
                    // Lê registro
                    movePonteiroRRN(arqBin, RRN);
                    inicializarRegistro(registro);
                    if (lerRegistro(arqBin, registro)) break;
                    codEst = registro->codEstacao;
                    
                    // Compara demais campos
                    if (buscaRegistro(registro, linhaBusca, qtdCampos)){  

                        /* Chama função atualizarHeader() como se uma remoção houvesse acontecido,
                        pois se os campos "nomeEstacao", "codEstacao" ou "codProxEstacao" forem alterados, 
                        existe a possibilidade de que o o total número de estações ou o total número de pares seja afetado.*/
                        atualizarHeader(cabecalho, dados, registro, rmv);
                        atualizarRegistro(registro, linhaAtualizacoes, qtdAtualizacoes);

                        // Atualizando arquivo índices 
                        if (codEst != (registro->codEstacao)) // se o codEstacao foi alterado, é preciso modificar o arquivo índice
                            atualizaCodEstacaoIndex(arqBinIndex, codEst, registro->codEstacao);

                        // Escreve registro atualizado
                        movePonteiroRRN(arqBin, RRN);
                        escreverRegistro(arqBin, registro);
                        atualizarHeader(cabecalho, dados, registro, insert);
                    } else break;
                }
            }
        }

        // De alguma forma, a busca por codEstacao foi realizada, então não realiza a busca normal
        if(RRN != -2){
            desalocaVetorDePonteiros(linhaBusca, qtdCampos*2);
            desalocaVetorDePonteiros(linhaAtualizacoes, qtdAtualizacoes*2);
            continue;
        }

        /*       # Busca Normal #       */
        RRN = -1; fseek(arqBin, tamCabecalho, SEEK_SET); 
        while(check_eof(arqBin)){
            RRN++; 

            inicializarRegistro(registro);
            if(lerRegistro(arqBin, registro)) continue;

            codEst = registro->codEstacao;
            // Compara campos
            if(buscaRegistro(registro, linhaBusca, qtdCampos)){
                atualizarHeader(cabecalho, dados, registro, rmv); 
                atualizarRegistro(registro, linhaAtualizacoes, qtdAtualizacoes);

                //Atualizando arquivo índice
                if (codEst != (registro->codEstacao)) // se o codEstacao foi alterado, é preciso modificar o arquivo índice
                        atualizaCodEstacaoIndex(arqBinIndex, codEst, registro->codEstacao);

                // Escreve registro atualizado
                movePonteiroRRN(arqBin, RRN);
                escreverRegistro(arqBin, registro);
                atualizarHeader(cabecalho, dados, registro, insert);
            }
        }

        desalocaVetorDePonteiros(linhaBusca, qtdCampos*2);
        desalocaVetorDePonteiros(linhaAtualizacoes, qtdAtualizacoes*2);

    }

    cabecalho->status = consistente;
    escreverHeader(arqBin, cabecalho);
    cabecalhoIndex->status = consistente;
    escreverHeaderIndex(arqBinIndex, cabecalhoIndex);

    fclose(arqBin);
    fclose(arqBinIndex);
    free(registro);
    free(cabecalho);
    
    BinarioNaTela(inBin);
    BinarioNaTela(inBinIndex);

}