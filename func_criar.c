#include "funcionalidades.h"

void funcUm(dadosHeader *dados){

    header *cabecalho = criarHeader();
    reg *registro = malloc(sizeof(reg));
    verificarReg(registro);

    // Lê os nomes dos arquivos .csv e .bin
    char inCsv[100], outBin[100];
    scanf("%s %s", &inCsv, &outBin);

    /*   #   Abrindo e Verificando Arquivos    # */

    FILE *arqCsv = fopen(inCsv, "r");
    verificarArq(arqCsv);
    pularCabecalhoCsv(arqCsv);

    FILE *arqBin = fopen(outBin, "wb");
    verificarArq(arqBin);


    /*   #   Registro no arqCsv -> Registro no arqBin   # */

    // Muda status do arquivo para inconsistente enquanto são realizadas escritas
    cabecalho->status = inconsistente;

    // Escreve um cabeçalho inicial no arquivo bin
    escreverHeader(arqBin, cabecalho);

    while(check_eof(arqCsv)){
        inicializarRegistro(registro);                  // "Zera" campos do registro 
        montarRegistro(arqCsv, registro);               // Copia registro do csv para a struct registro
        atualizarHeader(cabecalho, dados, registro, 1); // Atualiza cabeçalho indicando que foi feita uma escrita
        escreverRegistro(arqBin, registro);             // Escreve registro no binário
    }

    cabecalho->status = consistente;

    // Escreve o cabeçalho final no arquivo bin
    escreverHeader(arqBin, cabecalho);
   
    /*   #   Fechando Arquivos e BinarioNaTela()    # */

    free(registro);
    free(cabecalho);
    fclose(arqCsv);
    fclose(arqBin);

    BinarioNaTela(outBin);
}

void funcCinco(){
    /*      # structs #     */
    reg *registro = malloc(sizeof(reg)*1);
    verificarReg(registro);
    header *cabecalho = malloc(sizeof(header)*1);
    verificarHeader(cabecalho);

    headerIndex *cabecalhoIndex = malloc(sizeof(headerIndex)*1);
    verificarHeaderIndex(cabecalhoIndex);

    /*      # User Input #      */
    char inBin[100], inBinIndex[100];

    scanf("%s", &inBin);
    scanf("%s", inBinIndex);

    /* # Abertura e verificação de arquivos # */ 
    FILE *arqBin = fopen(inBin, "rb");
    FILE *arqBinIndex = fopen(inBinIndex, "wb");
    verificarArq(arqBin);
    verificarArq(arqBinIndex);

    lerCabecalho(arqBin, cabecalho);

    // vetor de registros do arquivo índice
    int capacidade = 10;
    regIndex *vetRegistroIndex = malloc(sizeof(regIndex)*capacidade);

    // Loop até o final do arquivo binario
    int i = 0;
    int rrnAtual = -1;
    while(check_eof(arqBin)){
        rrnAtual++;

        // Se a capacidade for atingida, aloca mais memória
        if(i >= capacidade-1){
            capacidade *= 2;
            vetRegistroIndex = realloc(vetRegistroIndex, sizeof(regIndex)*capacidade);
        }

        // Lê registro e guarda Estação e RRN no vetor
        if(lerRegistro(arqBin, registro)) {
            continue;
        }

        vetRegistroIndex[i].codEstacao = registro->codEstacao;
        vetRegistroIndex[i].RRN = rrnAtual;

        i++;
    }

    ordenarIndiceHeap(vetRegistroIndex, i);

    // Escreve registros no arquivo índice
    cabecalhoIndex->status = inconsistente;
    fwrite(&(cabecalhoIndex->status), sizeof(char), 1, arqBinIndex);

    for(int j=0; j<i; j++){
        fwrite(&(vetRegistroIndex[j].codEstacao), sizeof(int), 1, arqBinIndex);
        fwrite(&(vetRegistroIndex[j].RRN), sizeof(int), 1, arqBinIndex);
    }

    cabecalhoIndex->status = consistente;
    fseek(arqBinIndex, 0, SEEK_SET);
    fwrite(&(cabecalhoIndex->status), sizeof(char), 1, arqBinIndex);


    /*   #   Fechando Arquivos e BinarioNaTela()    # */

    free(registro);
    free(cabecalho);
    free(vetRegistroIndex);
    free(cabecalhoIndex);
    fclose(arqBin);
    fclose(arqBinIndex);

    BinarioNaTela(inBinIndex);
}


