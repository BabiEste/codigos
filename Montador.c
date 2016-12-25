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

    //Vari�veis tempor�rias para salvar no EQU
    char carac;
    int num;

    char quebra[]=" ,\n\t";
    char mnemonicos[]="/MOV/ADD/SUB/CMP/JMP/JC/JNC/JZ/JNZ/JBE/JA/CALL/RET/HLT/INC/DEC/";
    char doisBytes[]="/MOV/ADD/SUB/CMP/JMP/JC/JNC/JZ/JNZ/JBE/JA/CALL/";
    char umByte[]="/INC/DEC/RET/HLT/";

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

            }
        }
    linha++;
    }

    fclose(entrada);
    fclose(saida);
}


