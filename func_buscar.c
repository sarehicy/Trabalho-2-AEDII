#include "funcionalidades.h"

void funcDois(){
    reg *registro = malloc(sizeof(reg));
    verificarReg(registro);

    header *cabecalho = malloc(sizeof(header));
    verificarHeader;

    // Lê nome do arquivo bin
    char inBin[100];
    scanf("%s", inBin);
    
    FILE *arqBin = fopen(inBin, "rb");
    verificarArq(arqBin);
    lerCabecalho(arqBin, cabecalho);

    /*   #   Leitura e Impressão dos Registros   # */

    while(check_eof(arqBin)){
        inicializarRegistro(registro);      // "Zera" campos da struct antes da leitura para não misturar informações
        if (!lerRegistro(arqBin, registro)) // Tenta ler registro do arquivo bin
            imprimirRegistro(registro);     // Se conseguir ler, imprime registro
    } 

    free(registro);
    fclose(arqBin);
}

void funcTres(){
    reg *registro = malloc(sizeof(reg));
    verificarReg(registro);

    header *cabecalho = malloc(sizeof(header));
    verificarHeader;
   
    char inBin[100];            
    int qtdBuscas;              
    int qtdCampos;

    
    scanf("%s", inBin);
    scanf("%d", &qtdBuscas);

    // # Abertura e verificação de arquivos # // 
    
    FILE *arqBin = fopen(inBin, "rb");
    verificarArq(arqBin);
    lerCabecalho(arqBin, cabecalho);


    //      # Realização das buscas #         // 

    // Loop 1: Realiza uma busca.
    for (int i = 0; i < qtdBuscas; i++){
        
        scanf("%d", &qtdCampos);
        char *linha[qtdCampos*2];           // Vetor com os *pares "campo" e "valor" 

        montarBusca(linha, qtdCampos);

        // Quantidade de vezes que a busca foi bem sucedida
        // É usado para controlar a mensagem de erro "Registro não encontrado"
        int buscaSucesso = 0;

        while(check_eof(arqBin)){
            inicializarRegistro(registro);

            if (lerRegistro(arqBin, registro))
                continue;

            if (buscaRegistro(registro, linha, qtdCampos)){
                imprimirRegistro(registro);
                buscaSucesso++;
            }
        }

        // Se não houver sucesso na busca, imprime a mensagem "Registro inexistente"
        if (buscaSucesso == 0)
                printf("Registro inexistente.\n");

        // Imprime uma quebra de linha caso não seja a última iteração
        if(i != qtdBuscas-1)
            printf("\n");
        
        //Retorna o ponteiro para o primeiro registro para realizar uma nova busca
        fseek(arqBin, 17, SEEK_SET);
        desalocaVetorDePonteiros(linha, qtdCampos*2);
    }

    free(registro);
    fclose(arqBin);
}

void funcQuatro(){
    reg *registro = malloc(sizeof(reg));

    header *cabecalho = malloc(sizeof(header));
    verificarHeader;

    // Lê nome do arquivo binário e RRN
    char inBin[100];
    int RRN;

    scanf("%s %d", &inBin, &RRN);

    /*----------------------------------------------------------
                 ABRINDO E VERIFICANDO ARQUIVOS
    ----------------------------------------------------------*/

    FILE *arqBin = fopen(inBin, "rb");
    verificarArq(arqBin);
    lerCabecalho(arqBin, cabecalho);
    
    /*----------------------------------------------------------
                 LENDO E IMPRIMINDO REGISTRO
    ----------------------------------------------------------*/

    movePonteiroRRN(arqBin, RRN);   // Move o ponteiro para o registro de RRN informado

    inicializarRegistro(registro);      // Inicializa registro com valores padrão
    if(!lerRegistro(arqBin, registro))  // Tenta ler registro do RRN indicado
        imprimirRegistro(registro);     // Se conseguir ler, imprime o registro
    else
        printf("Registro inexistente.\n");  // Se não, imprime mensagem de erro


    free(registro);
    fclose(arqBin);
}

void funcSeis(){
    /*      # structs #     */
    reg *registro = malloc(sizeof(reg));
    verificarReg(registro);

    header *cabecalho = malloc(sizeof(header));
    verificarHeader;

    headerIndex *cabecalhoIndex = malloc(sizeof(headerIndex));
    verificarHeaderIndex(cabecalhoIndex);
   

    /*      # User Input #      */
    char inBin[100];            
    char inBinIndex[100];
    int qtdBuscas;              
    int qtdCampos;
    
    scanf("%s", inBin);
    scanf("%s", inBinIndex);
    scanf("%d", &qtdBuscas);

    
    // # Abertura e verificação de arquivos # // 
    
    FILE *arqBin = fopen(inBin, "rb");
    verificarArq(arqBin);
    FILE *arqBinIndex = fopen(inBinIndex, "rb");
    verificarArq(arqBinIndex);

    lerCabecalho(arqBin, cabecalho);
    lerCabecalhoIndex(arqBinIndex, cabecalhoIndex);


    //      # Realização das buscas #         // 

    for (int i = 0; i < qtdBuscas; i++){
        
        scanf("%d", &qtdCampos);
        char *linha[qtdCampos*2];           // Vetor com os *pares "campo" e "valor" 

        montarBusca(linha, qtdCampos);

        // Busca indexada 
        int RRN = -2;
        for(int j = 0; j<qtdCampos*2; j+=2){

            // Se algum campo for codEstacao faz a busca indexada            
            if(!(strcmp(linha[j], "codEstacao"))){       
                RRN = buscaIndexada(arqBinIndex, atoi(linha[j+1]));
                
                if(RRN != -1){ // Encontrou RRN de registro com codEstacao buscado

                    // Lê registro
                    movePonteiroRRN(arqBin, RRN);
                    inicializarRegistro(registro);
                    if (lerRegistro(arqBin, registro)){ //se não conseguir ler o registro
                        printf("Registro inexistente.\n");
                        break;
                    }

                    // Compara outros campos da busca
                    if(buscaRegistro(registro, linha, qtdCampos)){
                        imprimirRegistro(registro);
                        if(i != qtdBuscas-1) printf("\n"); //Se não for a última busca, imprime quebra de linha
                    }
                    else{  // Se os outros campos forem diferentes
                        printf("Registro inexistente.\n");
                    }
                } 
                else{ // Se não encontrou
                    printf("Registro inexistente.\n");
                }

                break;
            }
        }

        // De alguma forma, a busca por codEstacao foi realizada, então não realiza a busca normal
        if(RRN != -2){
            desalocaVetorDePonteiros(linha, qtdCampos*2);
            continue;
        }
    

        // Quantidade de vezes que a busca foi bem sucedida
        // É usado para controlar a mensagem de erro "Registro não encontrado"
        int buscaSucesso = 0;

        // Busca por campo 
        while(check_eof(arqBin)){
            inicializarRegistro(registro);

            if (lerRegistro(arqBin, registro))
                continue;

            if (buscaRegistro(registro, linha, qtdCampos)){
                imprimirRegistro(registro);
                buscaSucesso++;
            }
        }

        // Se não houver sucesso na busca, imprime a mensagem "Registro inexistente"
        if (buscaSucesso == 0)
                printf("Registro inexistente.\n");

        // Imprime uma quebra de linha caso não seja a última iteração
        if(i != qtdBuscas-1)
            printf("\n");
        
        //Retorna o ponteiro para o primeiro registro para realizar uma nova busca
        fseek(arqBin, 17, SEEK_SET);
        desalocaVetorDePonteiros(linha, qtdCampos*2);
    }

    free(registro);
    free(cabecalho);
    free(cabecalhoIndex);
    fclose(arqBin);
    fclose(arqBinIndex);
}
