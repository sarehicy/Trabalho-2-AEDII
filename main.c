//Larry Felipe Silva Gonçalves NUSP: 16966460
//Sarah Meicy Machado Barbosa NUSP: 16830357

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares.h"
#include "cabecalhos.h"
#include "arquivos.h"
#include "funcionalidades.h"
#include "registros.h"
#include "structs.h"
int main(){    
    dadosHeader *dados = criarDadosHeader();

    int operacao;

    scanf("%d", &operacao);

    switch (operacao)
    {
    case 1: // Leitura de CSV e criação de BIN
        funcUm(dados);
        break;
    case 2: // Imprime todos os registros na tela
        funcDois();
        break;
    case 3: // Busca registros por campo
        funcTres();
        break;
    case 4: // Busca registro por RRN
        funcQuatro();
        break;
    case 5:
        funcCinco();
        break;
    case 6:
        funcSeis();
        break;
    case 7:  // Remove todos os registros que correspondem a uma busca
        funcSete(dados);
        break;
    case 8:
        funcOito(dados);
        break;
    case 9:
        funcNove(dados);
        break;
    default:
        break;
    }

    liberarDadosHeader(dados);

    return 0;
}
