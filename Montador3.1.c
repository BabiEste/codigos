//B�rbara Este Fernandez
//RA: 161025901

//Trabalho completo

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

//Inicializa��o dos arquivos:
FILE *entrada;
FILE *saida;

typedef struct { //EQUs
    char nome[101];
    int valor;
} equ;


int LimpaArgumento(char *token){ //Fun��o que retira os colchetes, indicadores e v�rgula se tiver no argumento. Retorna se o argumento � de um byte ou dois.
    int DoisBytes = 0;
    int i, j;
    
    printf("\n\tEntrou na funcao LimpaArgumento");
    if(strchr(token, ",")!=NULL)
    	DoisBytes = 1;
	
	for(i = 0; i < strlen(token); i++){
		if(token[i] == ',' || token[i] == '[' || token[i] == ']' || token[i] == 'A'){
			for(j = i; j < strlen(token); j++){
				token[j] = token[j+1];
			}
			i--;
		}	
	}
    return DoisBytes;
}

char LeEQU(char *token, equ equ[], int m){
	printf("\n\tEntrou na funcao LeEQU");
    int i;
    
    for(i = 0; i < m; i++){
        if (strcmp(equ[i].nome, token)==0);
        printf("\n\tValor do EQU %s = %s", token, equ[i].valor);
            return equ[i].valor;
    }
    printf("\nEQU %s nao encontrado", token);
    getch();
    exit(1);
}


char TransformaEmOpcode(char *token, char comando[], char *opcode){ //Transforma o mnem�nico em opcode
	printf("\n\tEntrou na funcao TransformaEmOpcode");
    if(strcmp(comando, "MOV")==0){
        if(token[0]=='A' && (strchr(token, "[")!=NULL))
            strcpy(opcode, "a0h");
        else if (token[0] == 'A')
            strcpy(opcode,  "b0h");
        else
            strcpy(opcode,  "a2h");
    }
    else if(strcmp(comando, "ADD")==0){
        if(strchr(token, "[")!=NULL)
        	strcpy(opcode,  "02h");
        else
        	strcpy(opcode, "04h");      
    }
    else if(strcmp(comando, "SUB")==0){
        if(strchr(token, "[")!=NULL) 
        	strcpy(opcode, "2ah");
        else 
        	strcpy(opcode, "2ch");   
    }
    else if(strcmp(comando, "CMP")==0){
        if(strchr(token, "[")!=NULL)
            strcpy(opcode, "3ah");
        else
            strcpy(opcode, "3ch");
    }
    printf("\n\tOPCODE de %s = %s", comando, opcode);
    return opcode;
}

//-----------------------------------------FUN��O PRINCIPAL-----------------------------------------------------------
int main () {
    //Vari�veis
    equ equ[101];
    char aux[101]; /*Armazena a linha de c�digo a ser lida*/
    char *token;
    char comando[3];
    char opcode[3];
    char argumento[101];
    int DoisBytes;
    char carac[101];
    int num;
    int linha = 1;
    int m = 0 /*Contador de EQUs*/;

    //Mnem�nicos
    char mnemonico[]= "MOV/ADD/SUB/CMP";

    //Verifica��o dos arquivos
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
    
    
    rewind(entrada);

    while(fgets(aux, 100, entrada)!= NULL){ //Enquanto n�o � o final do arquivo
        printf("\nFrase auxiliar: %s", aux);

        token = strtok(aux, " ");

        while (token != NULL){
        	printf("\nToken: %s", token);
            if(aux[0]!=' '){ //Se o primeiro indice da linha n�o � um espa�o em branco, � um EQU (PARTE 1)
                if (atoi(token)!=0) { //Se � diferente de zero � porque � o valor de um EQU
                	printf("\nEntrou na condicao de ser numero");
                	num = atoi(token);
                	//----------------------------------N�O CONSIGO PASSAR O VALOR PARA O EQU---------------------------------------------
                	equ[m].valor = num;
                	
				}    
                else { 	//� o nome da EQU
                    strcpy(carac, token);
                    if (strcmp(token,"EQU")!=0) { //Se � uma palavra mas n�o � EQU, � o nome da EQU
                        strcpy(equ[m].nome, carac);
                    }
                }
                printf("\nEQU[%d] salva: \n\tNome: %s\n\tValor:", m, equ[m].nome, equ[m].valor); 
            }
            else { //Se n�o � EQU, ent�o � um comando ou argumento de comando (PARTE 2)
            	printf("\n\tLINHA %d", linha);
                if(strstr(mnemonico, token)==NULL){ //Se n�o � um comando, � um argumento
                	printf("\nToken: %s", token);
                    TransformaEmOpcode(token, comando, opcode);
                    DoisBytes = LimpaArgumento(token); //Limpa o argumento, para ficar s� com o seu valor
                    printf("\nValor do Token: %s", token);
                    if (DoisBytes == 1)
                        linha = linha+2; //Pula duas linhas porque s�o dois argumentos por linha
                    else
                    	linha++;
                    if (isalpha(*token)!=0){ //Se � caracter, ent�o um EQU foi utilizado
                        *token = LeEQU(token, equ, m);
                        fseek(saida, 0, SEEK_END);
                        fputs(opcode, saida);
                        fputc('\n', saida);
                        fputs(token, saida);
                        fputc('\n',saida);
                    }

                }
                else{ //Guarda o comando para ser usado na leitura do argumento
                	printf("\nToken: %s", token);
                	strcpy(comando, token);
                }
            }
            printf("\nTERMINOU UM TOKEN\n\n\n");
        token = strtok(NULL, " "); //Nova linha
        
        } /*while (token != NULL)*/ 
    m++;
    } /*while(fgets(aux, 100, entrada) != EOF)*/
    fclose(entrada);
    fclose(saida);
}
