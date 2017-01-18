//B�rbara Este Fernandez
//RA: 161025901

//Trabalho completo

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

#include <locale.h>

//Inicializa��o dos arquivos:
FILE *entrada;
FILE *saida;

typedef struct { //EQUs e r�tulos
    char nome[101];
    int valor;
} rotulos;

void calcula_mem(char aux[], int *mem){
	char *token;
	
	
	if(aux[0] == ' ') { /*N�o � r�tulo, � mnem�nico*/
		token = strtok(aux, " \n");
		/*Se tiver mnem�nico, aumenta o valor de MEM*/
	} 
	else { /*Possui um r�tulo na frente*/
		token = strtok(aux, " \n"); /*Ignora o r�tulo*/
		token = strtok(NULL, " \n"); /*L� o mnem�nico na segunda palavra*/
		if(token == NULL) return; /*S� tem r�tulo na linha*/
	}	
	
	
	if( strstr("RET#HLT", token) != NULL){
    	*mem = *mem + 1;
	}
    else if( strstr("MOV#ADD#SUB#CMP#JMP#JC#JNC#JZ#JNZ#JBE#JA#CALL", token) != NULL){
    	
    	
    	
    	token = strtok(NULL, " ,\n"); /*Passa para o primeiro argumento*/
    	if ((strcmp("B", token)) == 0 || (strcmp("[B]", token)) == 0) {
    		*mem = *mem + 1;
		}
		else {
			token = strtok(NULL, " ,\n"); /*Passa para o segundo argumento*/
			printf("x%sx\n ", token);
			if(token == NULL) {
				*mem = *mem + 2;
				return;
			}
			
			if ((strcmp("B", token)) == 0 || (strcmp("[B]", token)) == 0){
				*mem = *mem + 1;
			} 
			else {
				*mem = *mem + 2;	
			}	
		} 
	}
}

void remove_comentario(char aux[]){
	int i;
	for(i = 0; i < strlen(aux); i++){
		if(aux[i] == ';')
			aux[i] = '\0';
	}
}

void le_rotulo(rotulos rot[], char aux[], int *nr, int mem){
	char *token;
	
	token = strtok(aux, " \n"); /*L� o nome do r�tulo*/
	if (token[strlen(token)-1] == '\n') /*Retira o ENTER*/
		token[strlen(token)-1] = '\0';
	strcpy(rot[*nr].nome, token); /*Coloca na struct o nome do r�tulo*/
	//printf("x%sx ", rot[*nr].nome);
	 
	token = strtok(NULL, " \n"); /*L� a segunda palavra*/
	if(token != NULL){
		if(strcmp(token, "EQU") == 0) { /*Verifica se � 'EQU'*/
			token = strtok(NULL, " \n"); /*L� a terceira palavra*/
			rot[*nr].valor = atoi(token);
		}
		else { /*Se n�o � 'EQU' ent�o � r�tulo com mnem�nico na frente*/
			rot[*nr].valor = mem;
		}
	}
	else { /*Se token � nulo ent�o � r�tulo sem mnem�nico na frente*/
		rot[*nr].valor = mem;
	}
	
	//	printf("[%d] x%sx", rot[*nr].valor, rot[*nr].nome);
	
	*nr = *nr + 1;
}


//Fun��es do Passo 2



int procura_rotulo(rotulos rot[], char nome, int nr, int linha){
	int i;
	for(i = 0; i < nr; i++){
		if(strcmp(rot[i].nome, nome) == 0){
			return rot[i].valor;
		}
	}
	printf("\nERRO: [Linha %d] R�tulo %s n�o encontrado!", linha, nome);
	return -1;
}




int transforma_opcode(char aux[], char *opcode) {
	char *token;
	
	if(aux[0] != ' ') { /*Se possui um r�tulo no in�cio, ignora-o*/
		token = strtok(NULL, " \n");
	}
	
	token = strtok(NULL, " \n"); 
	if (token == NULL) return "oi"; /*Se n�o h� mnem�nico na linha, sai da fun��o*/
	printf("MNEM�NICO: %s", token);
	if (strcmp(token, "MOV") == 0) {
		token = strtok(NULL, ", \n"); /*Passa para o primeiro operador*/
		if((strcmp(token, "[A]") != 0) || (strcmp(token, "[B]") != 0)) { /*Se o operador n�o � [A] ou [B]*/
			//verificar se � n�mero ou r�tulo
		}
	}
}














//-----------------------------------------FUN��O PRINCIPAL-----------------------------------------------------------
int main () {
	
	setlocale(LC_ALL,"portuguese");
    //Vari�veis
    rotulos rot[101];
	int linha = 0, mem = 0 /*Conta os bytes usandos nos comandos*/, nr = 0 /*Contador de r�tulos*/;
    char aux[101] /*Armazena a linha de c�digo a ser lida*/, aux2[101] /*Auxiliar da auxiliar para strtok*/;
    char *opcode;
	int i; 
	
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
	
	/*Passo 1*/
    while(fgets(aux, 101, entrada)!= NULL){ //Enquanto n�o � o final do arquivo
        //printf("\nFrase auxiliar: [%d] x%sx", linha, aux);
      	remove_comentario(aux);
      	
      	if (strlen(aux) != 0 && aux[0] != '\n') {
      		strcpy(aux2, aux);
	        if(aux[0] != ' ' && strlen(aux)-1 != 0)
	        	le_rotulo(rot, aux2, &nr, mem);
	        	strcpy(aux2, aux);
	        calcula_mem(aux, &mem);	
		}
        
		linha++;
	}
	/*Passo 2*/
	
	rewind(entrada); /*Ler novamente o arquivo*/
	while(fgets(aux, 101, entrada) != NULL){
		remove_comentario(aux);
		
		if(strlen(aux) != 0 && aux[0] != '\n'){
			strcpy(aux2, aux);
			transforma_opcode(aux2, &opcode);
		}
	}
	
}
