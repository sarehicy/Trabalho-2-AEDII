#include "auxiliares.h"

void montarBusca(char **linha, int qtdCampos){

    for (int i = 0; i < (qtdCampos*2); i+=2){

            linha[i] = malloc(sizeof(char)*50);      
            linha[i+1] = malloc(sizeof(char)*50);     
            if (!linha[i] || !linha[i+1]) exit(0);
            
            scanf("%s", linha[i]);          // Lê o campo 
            
            if(!strcmp(linha[i], "nomeEstacao") || !strcmp(linha[i], "nomeLinha")){
                ScanQuoteString(linha[i+1]);
            }
            else{
                scanf("%s", linha[i+1]);
                if (!strcmp(linha[i+1], "NULO")){
                    strcpy(linha[i+1], "-1");
                }
            }
    }
}

void desalocaVetorDePonteiros(char **v, int n){
    if (!v) return;

    for (int i = 0; i<n; i++){
        free(v[i]);
    }
}


void ScanQuoteString(char *str) {
    char R;

    while ((R = getchar()) != EOF && isspace(R))
        ; // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n') { // campo NULO
        getchar();
        getchar();
        getchar();       // ignorar o "ULO" de NULO.
        strcpy(str, ""); // copia string vazia
    } else if (R == '\"') {
        if (scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar();         // ignorar aspas fechando
    } else if (R != EOF) { // vc tá tentando ler uma string que não tá entre
                           // aspas! Fazer leitura normal %s então, pois deve
                           // ser algum inteiro ou algo assim...
        str[0] = R;
        scanf("%s", &str[1]);
    } else { // EOF
        strcpy(str, "");
    }
}

void printInt(int n){
    // Ao imprimir um registro na tela:
    // Se o campo é -1 -> imprime "NULO"
    if (n == -1)
        printf("NULO");

    // Caso contrário, imprime o valor do campo
    else
        printf("%d", n);
}

void printString(char string[], int tamanho){
    // Ao imprimir um registro na tela:
    // Se a string é vazia -> imprime "NULO"
    if (tamanho == 0)
        printf("NULO");

    // caso contrário, imprime o valor do campo
    else
        printf("%s", string);
}

void movePonteiroRRN(FILE *arq, int rrn){

    // Move o ponteiro até o campo "proxRRN" no cabecalho
    fseek(arq, 5, SEEK_SET);

    // lê o proxRRN no arquivo
    int proxrrn;
    fread(&proxrrn, sizeof(int), 1, arq);

    // Verifica se o RRN buscado existe no arquivo
    if(rrn >= proxrrn){
        printf("Registro inexistente.");
        exit(0);
    }

    // Se existe, move o ponteiro para o RRN
    fseek(arq, (tamCabecalho+(tamRegistro*rrn)), SEEK_SET);
}

void pularCabecalhoCsv(FILE *arq){
    verificarArq(arq);

    char *header = malloc(sizeof(char)*256);

    // Verificando alocação da string
    if(!header){
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }

    size_t n = 256;
    
    getline(&header, &n, arq);                  // lê cabeçalho movendo ponteiro para depois do cabeçalho


    free(header);
}

void BinarioNaTela(char *arquivo) {
    FILE *fs;
    if (arquivo == NULL || !(fs = fopen(arquivo, "rb"))) {
        fprintf(stderr,
                "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): "
                "não foi possível abrir o arquivo que me passou para leitura. "
                "Ele existe e você tá passando o nome certo? Você lembrou de "
                "fechar ele com fclose depois de usar?\n");
        return;
    }

    fseek(fs, 0, SEEK_END);
    size_t fl = ftell(fs);

    fseek(fs, 0, SEEK_SET);
    unsigned char *mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    unsigned long cs = 0;
    for (unsigned long i = 0; i < fl; i++) {
        cs += (unsigned long)mb[i];
    }

    printf("%lf\n", (cs / (double)100));

    free(mb);
    fclose(fs);
}

char check_eof (FILE* f){
	char ch = 0;
	ch = fgetc(f);
	fseek(f, -1, SEEK_CUR);
	return (ch==EOF)?0:1;
}

void ordenarIndiceHeap(regIndex *vetRegistroIndex, int n){

    for(int i = n / 2 - 1; i >= 0; i--)
        heapifyIndice(vetRegistroIndex, n, i);

    for(int i = n-1; i > 0; i--){
        troca(&(vetRegistroIndex[0]), &(vetRegistroIndex[i]));

        heapifyIndice(vetRegistroIndex, i, 0);
    }
}

void heapifyIndice(regIndex *vetRegistroIndex, int n, int i){
    //maior como raiz
    int maior = i;
    int esq = 2*i + 1;
    int dir = 2*i + 2;

    // Verifica filhos maiores que a raiz
    if(esq < n && vetRegistroIndex[esq].codEstacao > vetRegistroIndex[maior].codEstacao){
        maior = esq;
    }
    if(dir < n && vetRegistroIndex[dir].codEstacao > vetRegistroIndex[maior].codEstacao){
        maior = dir;
    }

    // Se o maior não é a raiz
    if(maior != i){

        troca(&(vetRegistroIndex[i]), &(vetRegistroIndex[maior]));

        heapifyIndice(vetRegistroIndex, n, maior);
    }
}

void troca(regIndex *a, regIndex *b){
    regIndex temp = *a;
    *a = *b;
    *b = temp;
}


int buscaIndexada(FILE *arq, int codEstacao){
    verificarArq(arq);
    //malloca um registro=
    regIndex *registro = malloc(sizeof(regIndex)); verificarRegIndex(registro);

    // Verifica quantidade de registros
    fseek(arq, 0, SEEK_END);
    int tamanhoArq = ftell(arq);

    // Se o arquivo não conter registros, não realiza busca
    if (tamanhoArq <=1) return -1;

    // final do arquivo - 1(tamanho do cabeçalho) / 8(tamanho do registro)
    int qtdRegistros = (tamanhoArq - 1)/8;

    int esq = 0;
    int dir = qtdRegistros - 1;

    // Faz busca binária
    while(esq <= dir){

        // RRN do meio
        int meio = (esq + dir) / 2;
        
        // Calculo da posição do RRN
        fseek(arq, 1+(meio*8), SEEK_SET);

        //Lê registro
        lerRegistroIndex(arq, registro);

        // se encontrou, retorna o RRN
        if(registro->codEstacao == codEstacao){
            int rrn = registro->RRN;
            free(registro);
            return rrn;
        }
        //senao continua buscando
        if(codEstacao < registro->codEstacao){
        dir = meio - 1;
        }
        else{
            esq = meio + 1;
        }
    }

    free(registro);
    //se nao achou retorna -1
    return -1;
}

void atualizaCodEstacaoIndex(FILE *arq, int codEstAntigo, int codEstNovo){
    buscaIndexada(arq, codEstAntigo); // Move ponteiro para depois do registro com codEstacao especificado
    fseek(arq, -8, SEEK_CUR);         // Move ponteiro para o começo do registro desejado

    // Escreve novo codEstacao
    fwrite(&(codEstNovo), sizeof(int), 1, arq);
    
    reordenarArqIndex(arq);
}

FILE *removeRegistroIndex(char *nomeArq, int RRN){
    FILE *arqOrigem = fopen(nomeArq, "rb"); verificarArq(arqOrigem);
    FILE *arqDestino = fopen("temp.bin", "wb"); verificarArq(arqDestino);

    regIndex *registro = malloc(sizeof(regIndex));
    verificarRegIndex(registro);

    /* # Leitura do status do arquivo índice #*/
    char status;
    fread(&(status), sizeof(char), 1, arqOrigem);
    fwrite(&(status), sizeof(char), 1, arqDestino);

    /* # Leitura dos registros de dados do arquivo índice #*/
    while(check_eof(arqOrigem)){
        lerRegistroIndex(arqOrigem, registro);
        if (registro->RRN != RRN){
            fwrite(&(registro->codEstacao), sizeof(int), 1, arqDestino);
            fwrite(&(registro->RRN), sizeof(int), 1, arqDestino);
        }
    }

    fclose(arqOrigem);
    fclose(arqDestino);
    free(registro);

    remove(nomeArq); //deleta arquivo não modificado
    rename("temp.bin", nomeArq);
    arqDestino = fopen(nomeArq, "rb+");  verificarArq(arqDestino);
    
    return arqDestino;
}
