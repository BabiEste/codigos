//B�rbara Este Fernandez
//RA: 161025901

//Trabalho completo

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
    int DoisBytes = 0;
    int i;

    if(strstr(token,",")!=NULL){ //Verifica a exist�ncia de v�rgula, e a remove
        DoisBytes = 1;
        for(i = strstr(token, ","); i <= sizeof(token); i++){
            token[i] = token[i+1];
        }
    }
    if(strstr(token, "[")!=NULL){ //Colchetes
        for(i = strstr(token, "["); i <= sizeof(token); i++)
            token[i] = token[i+1];
        for(i = strstr(token, "]"); i <= sizeof(token); i++)
            token[i] = token[i+1];
    }
    if(strstr(token, "A")!=NULL){//Indicador
        for(i = strstr(token, "A"); i <= sizeof(token); i++){
            token[i] = token[i+1];
        }
    }
    return DoisBytes;
}

int LeEQU(char *token, equ equ[], int m){
    int i;
    for(i = 0; i <= m; i++){
        if (strcmp(equ[i].nome, token)==0);
            return equ[i].valor;
    }
    printf("\nRotulo %s nao encontrado", token);
}


char TransformaEmOpcode(char *token, char argumento[]){ //Transforma o mnem�nico em opcode
    if(strcmp(token, "MOV")==0){
        if(argumento[0]=='A' && (strstr(argumento, "[")!=NULL))
            return "a0h";
        else if (argumento[0] == 'A')
            return "b0h";
        else
            return "a2h";
    }
    else if(strcmp(token, "ADD")==0){
        if(strstr(argumento, "[")!=NULL)
            return "02h";
        else
            return "04h";
    }
    else if(strcmp(token, "SUB")==0){
        if(strstr(argumento, "[")!=NULL)
            return "2ah";
        else
            return "2ch";
    }
    else if(strcmp(token, "CMP")==0){
        if(strstr(argumento, "[")!=NULL)
            return "3ah";
        else
            return "3ch";
    }
    else if(strcmp(token, "JMP")==0)
        return "ebh";
    else if(strcmp(token, "JC")==0)
        return "72h";
    else if(strcmp(token, "JNC")==0)
        return "73h";
    else if(strcmp(token, "JZ")==0)
        return "74h";
    else if(strcmp(token, "JNZ")==0)
        return "75h";
    else if(strcmp(token, "JBE")==0)
        return "76h";
    else if(strcmp(token, "JA")==0)
        return "77h";
    else
        printf("\nO comando %s eh invalido", token);
}

//-----------------------------------------FUN��O PRINCIPAL-----------------------------------------------------------
int main () {
    if((entrada = fopen("entrada.txt", "rt")) == NULL){
        printf("\nErro na inicializacao do arquivo de mnemonicos.");
        getch();
        exit(1);
    }
    if((saida = fopen("saida.txt", "wt")) == NULL){
        printf("\nErro na abertura do arquivo de opcodes.");
        getch();
        exit(1);
    }

    //Vari�veis
    rotulos rotulo[101];
    equ equ[101];
    char *aux; /*Armazena a linha de c�digo a ser lida*/
    char *token;
    char opcode[3];
    int DoisBytes;

    //Vari�veis tempor�rias para salvar no EQU
    char *carac;
    char *argumento;
    int num;

    //Mnem�nicos
    char mnemonico[]= "#MOV#ADD#SUB#CMP#JMP#JC#JNC#JZ#JNZ#JBE#JA#CALL";

    int n = 0 /*�ndice do r�tulo*/, m = 0 /*Contador de EQUs*/, linha = 1;

    printf("ESSA PORRA TA FUNCINANDO");

    while((fgets(aux, 101, entrada)) != NULL) {

        printf("\nFrase auxiliar: %s", aux);

        token = strtok(aux, " ");
        printf("\nToken: %s", token);

        while (token != NULL) {
            if(aux[0]!=' '){ //Se o primeiro �ndice da linha n�o � um espa�o em branco, � porque � um r�tulo ou um EQU (PARTE 1)
                if (isalpha(token[0])== 0) //Se � zero � porque � n�mero
                    num = *token;
                else if (isalpha(token[0])!=0 && stricmp(token,"EQU")!=0) //Se � uma palavra mas n�o � EQU
                    strcpy(carac, token);

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
            else { //Se n�o � R�TULO nem EQU, ent�o � um comando ou argumento de comando(PARTE 2)
                if(strstr(token, mnemonico)==NULL){ //Se n�o � um comando, � um argumento
                    strcpy(argumento, token);
                    DoisBytes = LimpaArgumento(token);
                    if (DoisBytes == 1)
                        linha = linha+2; //Pula duas linhas porque s�o dois argumentos por linha
                    if (isalpha(*token)!=0){ //Se � caracter, ent�o um EQU foi utilizado
                        *token = LeEQU(token, equ, m);
                        fputs(token, saida);
                        putc('\n',saida);
                    }

                }
                else{ //Coloca o comando no arquivo de saida
                    strcpy(opcode, TransformaEmOpcode(token, argumento));
                    fputs(argumento, saida);
                    putc('\n',saida);
                    linha++;
                }
            }
        *token = strtok(NULL, " ");  //Nova linha
        }
    }
    fclose(entrada);
    fclose(saida);
}
