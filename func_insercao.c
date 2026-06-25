#include "funcionalidades.h"

void funcOito(dadosHeader *dados){
    /*      # structs #     */
    reg *registro = malloc(sizeof(reg));
    verificarReg(registro);

    header *cabecalho = malloc(sizeof(header));
    verificarHeader(cabecalho);

    headerIndex *cabecalhoIndex = malloc(sizeof(headerIndex));
    verificarHeaderIndex(cabecalhoIndex);

    regIndex *registroIndex = malloc(sizeof(regIndex));
    verificarRegIndex(registroIndex);

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

    lerCabecalho(arqBin, cabecalho);
    lerCabecalhoIndex(arqBin, cabecalhoIndex);

    montarDadosHeader(arqBin, registro, dados);
    dados->qtdPares = cabecalho->totalPares;
    /* ATENÇÃO!
        A funcão montarDadosHeader está contando os pares corretamente, no entanto no caso de teste 7
        a quantidade de pares do cabeçalho do arquivo está incorreta, o que acabava provocando erro no resultado final.
        Para evitar problemas a quantidade de pares da struct auxiliar foi igualada a quantidade de pares do cabeçalho. :)
        Conversado com o monitor Renan Catarin.
    */


    cabecalho->status = inconsistente; // porque vai escrever no arq
    escreverHeader(arqBin, cabecalho);
    cabecalhoIndex->status = inconsistente;
    escreverHeaderIndex(arqBinIndex, cabecalhoIndex);

    // Para cada inserção
    for(int i = 0; i<n; i++){
        // Se não tiver registros lógicamente removidos
        if(cabecalho->topo == -1){
            inicializarRegistro(registro);
            montarRegistroCmd(registro);

            registroIndex->codEstacao = registro->codEstacao;
            registroIndex->RRN = cabecalho->proxRRN;
            
            // Move ponteiro para o final do arquivo e insere novo registro
            fseek(arqBin, 0, SEEK_END);

            escreverRegistro(arqBin, registro);
            atualizarHeader(cabecalho, dados, registro, add);

            fseek(arqBinIndex, 0, SEEK_END);
            escreverRegIndex(arqBinIndex, registroIndex);
        }
        else{ // Inserção no meio do arquivo
            // Guarda RRN do registro onde será feita a inserção e o RRN que deve ir para o topo
            int RRNInserir = cabecalho->topo;
            movePonteiroRRN(arqBin, RRNInserir);
            lerRegistro(arqBin, registro);
            int RRNTopoNovo = registro->prox;

            // Montando o registro novo
            inicializarRegistro(registro);
            montarRegistroCmd(registro);
            registroIndex->codEstacao = registro->codEstacao;
            registroIndex->RRN = RRNInserir;

            // Inserindo novo registro
            movePonteiroRRN(arqBin, RRNInserir);
            escreverRegistro(arqBin, registro);

            cabecalho->topo = RRNTopoNovo;
            atualizarHeader(cabecalho, dados, registro, insert);

            fseek(arqBinIndex, 0, SEEK_END);
            escreverRegIndex(arqBinIndex, registroIndex);
        }
    }

    cabecalho->status = consistente;
    escreverHeader(arqBin, cabecalho);
    cabecalhoIndex->status = consistente;
    escreverHeaderIndex(arqBinIndex, cabecalhoIndex);

    // Depois de todas as inserções
    reordenarArqIndex(arqBinIndex);

    fclose(arqBin);
    fclose(arqBinIndex);
    free(registro);
    free(cabecalho);
    free(registroIndex);
    free(cabecalhoIndex);
    BinarioNaTela(inBin);
    BinarioNaTela(inBinIndex);
}
