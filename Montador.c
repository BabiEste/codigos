//B�rbara Este Fernandez
//RA: 161025901

//Trabalho parcial - Montador

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

//Inicializa��o e verifica��o dos arquivos:
FILE *entrada;
FILE *saida;

typedef struct { //R�tulos
    char nome[101];
    int valor;
} rotulos;

typedef struct { //EQUs
    char nome[101];
    int valor;
} equ;

int LimpaArgumento(char *token){ //Fun��o que retira os colchetes, indicadores e v�rgula se tiver no argumento. Retorna se o argumento � de um byte ou dois.
    const char virgula = ',';
    const char colchete_abre = '[';
    const char colchete_fecha = ']';
    const char indicador = 'A';
    int DoisBytes = 0;
    int i;

    if(strstr(*token, virgula)!=NULL){ //Verifica a exist�ncia de v�rgula, e a remove
        DoisBytes = 1;
        for(i = strstr(*token, virgula); i <= sizeof(*token); i++){
            token[i] = token[i+1];
        }
    }
    if(strstr(*token, colchete_abre)!=NULL){ //Colchetes
        for(i = strstr(token, colchete_abre); i <= sizeof(*token); i++)
            token[i] = token[i+1];
        for(i = strstr(*token, colchete_fecha); i <= sizeof(*token); i++)
            token[i] = token[i+1];
    }
    if(strstr(*token, indicador)!=NULL){//Indicador
        for(i = strstr(*token, indicador); i <= sizeof(*token); i++){
            token[i] = token[i+1];
        }
    }
    return DoisBytes;
}

int LeEQU(char *token, equ equ[], int m){
    int i;
    for(i = 0; i <= m; i++){
        if (strcmp(equ[i].nome, *token)==0);
            return equ[i].valor;
    }
    printf("\Rotulo %s nao encontrado", token);
}


char TransformaEmOpcode(char *token){
    //Precisa saber qual � o fucking argumento
}

//-----------------------------------------FUN��O PRINCIPAL-----------------------------------------------------------
int main () {
    if((entrada = fopen("entrada.txt", "rt")) == NULL){
        printf("\nErro na inicializacao do arquivo de mnemonicos.");
        getch();
        exit(1);
    }
    if((entrada = fopen("saida.txt", "wt")) == NULL){
        printf("\nErro na abertura do arquivo de opcodes.");
        getch();
        exit(1);
    }

    //Vari�veis
    rotulos rotulo[101];
    equ equ[101];
    char aux[101]; /*Armazena a linha de c�digo a ser lida*/
    char *token;
    int DoisBytes;

    //Vari�veis tempor�rias para salvar no EQU
    char carac;
    int num;

    //Mnem�nicos
    char mnemonico[]= "#MOV#ADD#SUB#CMP#JMP#JC#JNC#JZ#JNZ#JBE#JA#CALL";
    char quebra[]=" ,\n\t";

    int n = 0 /*�ndice do r�tulo*/, m = 0/*Contador de EQUs*/, linha = 1;

    while (fgets(aux, sizeof(aux), entrada) != NULL) {
        if (!aux[0]){ //Se o primeiro �ndice da linha n�o � nulo, � porque � um r�tulo ou um EQU (PARTE 1)
            *token = strtok(aux, " "); //Separa o r�tulo da frase inteira
            *token = toupper(*token);

            while(token){
                if (isalpha(token[0])== 0) //Se � zero � porque � n�mero
                    num = *token;
                else if (isalpha(token[0])!=0 && stricmp(token,"EQU")!=0) //Se � uma palavra mas n�o � EQU
                    strcpy(carac, *token);

                if (strcmp(token, "EQU")==0){ //Se � EQU, ent�o guarda na struct os valores nas vari�veis tempor�rias
                    strcpy(equ[m].nome,carac);
                    equ[m].valor = num;
                    m++;
                }
                else if (!(isalpha(*token))){ //Se � r�tulo, salva
                    strcpy(rotulo[n].nome, carac);
                    rotulo[n].valor = num;
                    n++;
                }
            }
        }
        else { //Se n�o � R�TULO nem EQU, ent�o � um comando ou argumento de comando(PARTE 2)
            token = strtok(aux," ");
            while (token) {
                if(strstr(token, mnemonico)==NULL){ //Se n�o � um comando, � um argumento
                    DoisBytes = LimpaArgumento(token);
                    if (DoisBytes == 1)
                        linha = linha+2; //Pula duas linhas porque s�o dois argumentos por linha
                    if (isalpha(*token)!=0){ //Se � caracter, ent�o um EQU foi utilizado
                        *token = LeEQU(*token, equ, m);
                        fputs(*token, saida);
                        fputs(quebra, saida);
                        *token = strtok(NULL, " ");  //Nova linha
                    }

                }
            }
        }
    linha++;
    }

    fclose(entrada);
    fclose(saida);
}
