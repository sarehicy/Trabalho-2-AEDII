#include "cabecalhos.h"

void verificarHeader(header *cabecalho){
    if(!cabecalho){
         printf("Falha no processamento do arquivo.\n");

         exit(0);
    }
}

void verificarHeaderIndex(headerIndex *cabecalho){
    if(!cabecalho){
         printf("Falha no processamento do arquivo.\n");
         exit(0);
    }
}


header *criarHeader(){
    header *cabecalho = malloc(sizeof(header));
    verificarHeader(cabecalho);

    // Define os valores default para o cabeçalho
    cabecalho->status = '0';
    cabecalho->topo = -1;
    cabecalho->proxRRN = 0;
    cabecalho-> totalEstacoes = 0;
    cabecalho-> totalPares = 0;

  return cabecalho;
}

void atualizarHeader(header *cabecalho, dadosHeader *tabela, reg *registro, int op){

    int qtdEst = tabela->qtdEstacoes;

    switch(op){
        case insert:
        case add: // Inserção de registro
            
            // Se inseriu no fim do arquivo
            if (op == add) (cabecalho->proxRRN)++;

            inserirPar(cabecalho, tabela, (registro->codEstacao), (registro->codProxEstacao));

            /*      # Tratando a Quantidade de Estações #       */
            //Se a estação já foi contabilizada, incrementa ocorrência e retorna da função
            for (int i = 0; i < qtdEst; i++){
                if (!strcmp(tabela->estacoes[i], registro->nomeEstacao)){
                    tabela->estacoesOcorrencias[i] = (tabela->estacoesOcorrencias[i])+ 1;
                    return;
                }
            }
            
            // Insere nova estação na tabela de estações 
            tabela->estacoes[qtdEst] = malloc(sizeof(char)*((registro->tamNomeEstacao) + 1));
            strcpy(tabela->estacoes[qtdEst], registro->nomeEstacao);
            tabela->estacoesOcorrencias[qtdEst] = 1;

            // Incrementa a quantidade total de estações
            (tabela->qtdEstacoes)++;
            cabecalho->totalEstacoes = tabela->qtdEstacoes;

            // Realoca memória para vetor de estações e vetor de ocorrências
            char **aux = realloc(tabela->estacoes, sizeof(char *) * ((tabela->qtdEstacoes) +1));
            if (!aux) exit(0);
            tabela->estacoes = aux;

            int *aux2 = realloc(tabela->estacoesOcorrencias, sizeof(int)* ((tabela->qtdEstacoes) +1) );
            if (!aux2) exit(0);
            tabela->estacoesOcorrencias = aux2;
            break;


        // Remoção de registro
        case rmv:
            removerPar(cabecalho, tabela, registro->codEstacao, registro->codProxEstacao);

            /* # Tratando a Quantidade de Estações */
            int i;
            for (i = 0; i<qtdEst; i++){

                // Busca estação do registro no vetor e decrementa a qtd de ocorrências
                 if (!strcmp(tabela->estacoes[i], registro->nomeEstacao)){
                    tabela->estacoesOcorrencias[i] = tabela->estacoesOcorrencias[i] - 1;
                    break;
                 }
            }

            // Se não existem mais ocorrências, a estação pode ser removida do vetor e a quantidade total de estações diminui
            if (tabela->estacoesOcorrencias[i] == 0){
                free(tabela->estacoes[i]);

                // Remove estação do vetor
                for (int j = i; j < (qtdEst-1); j++){
                    tabela->estacoes[j] = tabela->estacoes[j+1];
                    tabela->estacoesOcorrencias[j] = tabela->estacoesOcorrencias[j+1];
                }

                (tabela->qtdEstacoes)--;
                cabecalho->totalEstacoes = tabela->qtdEstacoes;

                // Realoca memória para vetor de estações e vetor de ocorrências
                char **aux = realloc(tabela->estacoes, sizeof(char *) * (tabela->qtdEstacoes +1));
                if(!aux) exit(0);
                tabela->estacoes = aux;

                int *aux2 = realloc(tabela->estacoesOcorrencias, sizeof(int)* (tabela->qtdEstacoes +1) );
                if (!aux2) exit(0);
                tabela->estacoesOcorrencias = aux2;
            }

            break;
    }



}



void inserirPar(header *cabecalho, dadosHeader *dados, int codEst, int codProx){
    // Se algum valor for nulo, o par não é valido
    if (codEst <= 0 || codProx <= 0) {
        return;}

    //Realoca Pares para caber mais um par
    int (*temp)[2] = realloc(dados->pares, (dados->qtdPares + 1) * sizeof(int[2]));
    if (!temp) exit(0);
    dados->pares = temp;

    // Adiciona o par aos dados 
    dados->pares[dados->qtdPares][0] = codEst;
    dados->pares[dados->qtdPares][1] = codProx;

    //e incrementa o total de pares nos dados e no cabeçalho
    (dados->qtdPares)++;

    (cabecalho->totalPares) = dados->qtdPares;


}



void removerPar(header *cabecalho, dadosHeader *dados, int codEst, int codProx){
    // Se algum valor for nulo, o par não é valido
    if (codEst <= 0 || codProx <= 0) return;

    // Descobre em que posição está o par
    int i;
    for(i = 0; i < dados->qtdPares; i++){
        // Encontrou o par
        if((dados->pares[i][0] == codEst) && (dados->pares[i][1] == codProx)){
                break;
        } 
    }  

    // Sobrescrevendo par
    for (int j = i; j < ((dados->qtdPares)-1); j++ ){
        dados->pares[j][0] = dados->pares[j+1][0];
        dados->pares[j][1] = dados->pares[j+1][1];
    }

    (dados->qtdPares)--;
    (cabecalho->totalPares) = dados->qtdPares;

    //Diminui tamanho do vetor pares
    int (*temp)[2] = realloc(dados->pares, (dados->qtdPares + 1) * sizeof(int[2]));
    if (!temp) exit(0);
    dados->pares = temp;
}


/*------------------------------------------------
                FUNCOES DADOS HEADER
    ------------------------------------------------*/

dadosHeader *criarDadosHeader(){
    dadosHeader *tabela = malloc(sizeof(dadosHeader));
    if (!tabela)
        return NULL;

    /*--------------------------------------
    Inicializando tabela com valores padrão
    ---------------------------------------*/

    tabela->estacoes = malloc(sizeof(char *));  // Vetor com espaço pra um ponteiro de char
    if (!(tabela->estacoes)) exit(0);

    tabela->estacoesOcorrencias = malloc(sizeof(int));
    if(!(tabela->estacoesOcorrencias)) exit(0);

    tabela->pares = malloc(sizeof(int[2]));
    if (!tabela->pares) exit(0);



    tabela->qtdEstacoes = 0;
    tabela->qtdPares = 0;

}

void montarDadosHeader(FILE *arqBin, reg *registro, dadosHeader *dados){
    header *aux = criarHeader();

    fseek(arqBin, tamCabecalho, SEEK_SET);

    while(check_eof(arqBin)){
        
        inicializarRegistro(registro);

        if (lerRegistro(arqBin, registro)) {
            continue;}
        atualizarHeader(aux, dados, registro, add);


    }

    free(aux);
}

void liberarDadosHeader(dadosHeader *dados) {
    for (int i = 0; i < dados->qtdEstacoes; i++) {
        free(dados->estacoes[i]);
    }

    free(dados->estacoes);
    free(dados->pares);
    free(dados);
}
