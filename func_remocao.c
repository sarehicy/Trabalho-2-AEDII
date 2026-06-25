#include "funcionalidades.h"

void funcSete(dadosHeader *dados){
    /*      # structs #     */
    reg *registro = malloc(sizeof(reg));
    verificarReg(registro);

    header *cabecalho = malloc(sizeof(header));
    verificarHeader(cabecalho);

    headerIndex *cabecalhoIndex = malloc(sizeof(headerIndex));
    verificarHeaderIndex(cabecalhoIndex);



    /*      # User Input #      */
    char inBin[100], inBinIndex[100]; int n;
    scanf("%s", inBin);
    scanf("%s",inBinIndex);
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

    int qtdCampos;
    while(n--){

        scanf("%d", &qtdCampos);
        char *linha[qtdCampos*2]; 
        montarBusca(linha, qtdCampos);

        int RRN = -2;    //Guarda RRN do registro cujos campos estão sendo comparados
    
        /*      # Busca por codEstacao #        */
        for (int i = 0; i < (qtdCampos*2); i+=2){
            if (!(strcmp(linha[i], "codEstacao"))){
                RRN = buscaIndexada(arqBinIndex, atoi(linha[i+1]));

                // registro com codEstacao encontrado
                if (RRN != -1){
                    // Lê registro
                    movePonteiroRRN(arqBin, RRN);
                    inicializarRegistro(registro);
                    if (lerRegistro(arqBin, registro)) break;

                    /// Compara demais campos
                    if(buscaRegistro(registro, linha, qtdCampos)){
                        registro->rem = '1';

                        registro->prox = cabecalho->topo;
                        cabecalho->topo = RRN;

                        // Move ponteiro para inicio do registro removido p reescrever registro marcado como logicamente removido
                        movePonteiroRRN(arqBin, RRN);
                        escreverRegistro(arqBin, registro);
                        atualizarHeader(cabecalho, dados, registro, rmv);
                        fclose(arqBinIndex);
                        arqBinIndex = removeRegistroIndex(inBinIndex, RRN);
                    } else break;

                } else break;
            }
        }

        // De alguma forma, a busca por codEstacao foi realizada, então não realiza a busca normal
        if (RRN != -2 ){
            desalocaVetorDePonteiros(linha, qtdCampos*2);
            continue;
        }

       /*       # Busca Normal #       */
        RRN = -1; fseek(arqBin, tamCabecalho, SEEK_SET); 
        while(check_eof(arqBin)){
            RRN++;

            // Lê registro
            inicializarRegistro(registro);
            if(lerRegistro(arqBin, registro))
                continue;
            
            //Se registro corresponder a busca, removê-lo
            if(buscaRegistro(registro, linha, qtdCampos)){
                registro->rem = '1';
                
                registro->prox = cabecalho->topo;
                cabecalho->topo = RRN;

                // Move ponteiro para inicio do registro de rrn removido p escrever
                movePonteiroRRN(arqBin, RRN);
                escreverRegistro(arqBin, registro);
                atualizarHeader(cabecalho, dados, registro, rmv);
                fclose(arqBinIndex);
                arqBinIndex = removeRegistroIndex(inBinIndex, RRN);
            }

        }

       desalocaVetorDePonteiros(linha, qtdCampos*2);
    }
    cabecalho->status = consistente;
    escreverHeader(arqBin, cabecalho);
    cabecalhoIndex->status = consistente;
    escreverHeaderIndex(arqBinIndex, cabecalhoIndex);

    fclose(arqBin);
    fclose(arqBinIndex);
    free(registro);
    free(cabecalho);
    free(cabecalhoIndex);
    
    BinarioNaTela(inBin);
    BinarioNaTela(inBinIndex);
}