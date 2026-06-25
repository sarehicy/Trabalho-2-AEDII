#include "registros.h"

void verificarReg(reg *registro){
    if(!registro){
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }
}

void verificarRegIndex(regIndex *registro){
    if(!registro){
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }
}

void inicializarRegistro(reg *registro){
    verificarReg(registro);
    
    // Inicializa o registro com os campos padrão

    // Campos de remoção
    registro->rem = '0';
    registro->prox = -1;

    // Campos de tamanho fixo (-1 Indica que não há dado)
    registro->codEstacao = -1;
    registro->codLinha = -1;
    registro->codProxEstacao = -1;
    registro->distProxEstacao = -1;
    registro->codLinhaInteg = -1;
    registro->codEstacaoInteg = -1;
    registro->tamNomeEstacao = -1;
    registro->tamNomeLinha = -1;

    // Campos de tamanho variável (Vazio)
    strcpy(registro->nomeLinha, "");
    strcpy(registro->nomeEstacao, "");
}

void imprimirRegistro(reg *registro){
    verificarReg(registro);

    // Imprime cada um dos campos com as funções auxiliares PrintInt e PrintString
    
    printInt(registro->codEstacao);
    printf(" ");
    printString(registro->nomeEstacao, registro->tamNomeEstacao);
    printf(" ");
    printInt(registro->codLinha);
    printf(" ");
    printString(registro->nomeLinha, registro->tamNomeLinha);
    printf(" ");
    printInt(registro->codProxEstacao);
    printf(" ");
    printInt(registro->distProxEstacao);
    printf(" ");
    printInt(registro->codLinhaInteg);
    printf(" ");
    printInt(registro->codEstacaoInteg);

    printf("\n");
}


int buscaRegistro(reg *registro, char **linha, int qtdCampos){

    for (int i = 0; i < (qtdCampos*2); i+=2){
                    
                // Compara campo do registro e da busca
                if(buscaCampo(registro, linha[i], linha[i+1]) == 1){
                    //Se os campos forem iguais,
                    //verifica se é o último campo da busca:
                    if(i >= qtdCampos*2 - 2){
                        return 1;
                    }
                    
                    //Se não for o último campo, continua comparando
                    continue;
                }
                else           // Se o campo do registro e o da busca for diferente, interrompe loop 4
                    break;     // continuando a busca no resto do arquivo
            }
}

int buscaCampo(reg *registro, char campo[], char valor[]){
    verificarReg(registro);

    /*
        PARA CADA CAMPO:
        1- Verifica o nome do campo
        2- Se for o campo certo, entra no if
        3- Compara o valor do campo buscado com o valor no registro
        4- Se for igual, retorna 1
        5- se for diferente, retorna 0
        6- Sai da função
    */

    // Codigo da Estacao
    if(!strcmp(campo, "codEstacao")){
        if(atoi(valor) == registro->codEstacao){
            return 1;
        }
    }
    
    //Nome da Estacao
    else if(!strcmp(campo, "nomeEstacao")){
        if(!strcmp((registro->nomeEstacao), valor)){
            return 1;
        }
    }
    
    // Codigo da Linha
    else if(!strcmp(campo, "codLinha")){
        if(atoi(valor) == registro->codLinha){
            return 1;
        }
    }
    
    // Nome da Linha
    else if(!strcmp(campo, "nomeLinha")){
        if(!strcmp((registro->nomeLinha), valor)){
            return 1;
        }
    }
    
    // Codigo da próxima Estacao
    else if(!strcmp(campo, "codProxEstacao")){
        if(atoi(valor) == registro->codProxEstacao){
            return 1;
        }  
    }
    
    // Distância da próxima Estacao
    else if(!strcmp(campo, "distProxEstacao")){
        if(atoi(valor) == registro->distProxEstacao){
            return 1;
        }
    }
    
    // Código da linha Integrada
    else if(!strcmp(campo, "codLinhaIntegra")){
        if(atoi(valor) == registro->codLinhaInteg){
            return 1;
        }
    }
    
    // Codigo da Estacao Integrada
    else if (!strcmp(campo, "codEstIntegra")){
        if(atoi(valor) == registro->codEstacaoInteg){
            return 1;
        }
    }

    return 0;
}

void atualizarRegistro(reg *registro, char **linha, int qtdAtualizacoes){
    verificarReg(registro);

    for(int i = 0; i<(qtdAtualizacoes*2); i+=2){

         // Codigo da Estacao
        if(!strcmp(linha[i], "codEstacao")){
            registro->codEstacao = atoi(linha[i+1]);
        }

        //Nome da Estacao
        else if(!strcmp(linha[i], "nomeEstacao")){
            strcpy(registro->nomeEstacao, linha[i+1]);
            registro->tamNomeEstacao = strlen(linha[i+1]);
        }

        // Codigo da Linha
        else if(!strcmp(linha[i], "codLinha")){
            registro->codLinha = atoi(linha[i+1]);
        }

        // Nome da Linha
        else if(!strcmp(linha[i], "nomeLinha")){
            strcpy(registro->nomeLinha, linha[i+1]);
            registro->tamNomeLinha = strlen(linha[i+1]);
        }

        // Codigo da próxima Estacao
        else if(!strcmp(linha[i], "codProxEstacao")){
            registro->codProxEstacao = atoi(linha[i+1]); 
        }

        // Distância da próxima Estacao
        else if(!strcmp(linha[i], "distProxEstacao")){
            registro->distProxEstacao = atoi(linha[i+1]);
        }

        // Código da linha Integrada
        else if(!strcmp(linha[i], "codLinhaIntegra")){
            registro->codLinhaInteg = atoi(linha[i+1]);

        }

        // Codigo da Estacao Integrada
        else if (!strcmp(linha[i], "codEstIntegra")){
            registro->codEstacaoInteg = atoi(linha[i+1]);
        }

    }
}


void montarRegistroCmd(reg *registro){  

    char buffer[100];

    scanf("%d", &(registro->codEstacao));

    ScanQuoteString(registro->nomeEstacao); 
    registro->tamNomeEstacao = strlen(registro->nomeEstacao);

    scanf("%s", buffer);
    if(!strcmp(buffer, "NULO")) registro->codLinha = -1;
    else registro->codLinha = atoi(buffer);

    ScanQuoteString(registro->nomeLinha); 
    registro->tamNomeLinha = strlen(registro->nomeLinha);

    scanf("%s", buffer);
    if(!strcmp(buffer, "NULO")) registro->codProxEstacao = -1;
    else registro->codProxEstacao = atoi(buffer);

    scanf("%s", buffer);
    if(!strcmp(buffer, "NULO")) registro->distProxEstacao = -1;
    else registro->distProxEstacao = atoi(buffer);

    scanf("%s", buffer);
    if(!strcmp(buffer, "NULO")) registro->codLinhaInteg = -1;
    else registro->codLinhaInteg = atoi(buffer);

    scanf("%s", buffer);
    if(!strcmp(buffer, "NULO")) registro->codEstacaoInteg = -1;
    else registro->codEstacaoInteg = atoi(buffer);

}
