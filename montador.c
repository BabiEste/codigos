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
    else if(strstr("MOV#ADD#SUB#CMP#JMP#JC#JNC#JZ#JNZ#JBE#JA#CALL", token) != NULL){
    	
    	token = strtok(NULL, " ,\n"); /*Passa para o primeiro argumento*/
    	
    	if ((strcmp("B", token)) == 0 || (strcmp("[B]", token)) == 0) {
    		*mem = *mem + 1;
		}
		else {
			token = strtok(NULL, " ,\n"); /*Passa para o segundo argumento*/
			//printf("x%sx\n ", token);
			if(token == NULL) {
				*mem = *mem + 2;
				return;
			}
			
			if (strcmp("B", token) == 0 || strcmp("[B]", token) == 0){
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



int procura_rotulo(rotulos rot[], char nome[], int nr, int linha){
	int i;
	for(i = 0; i < nr; i++){
		if(strcmp(rot[i].nome, nome) == 0){
			return rot[i].valor;
		}
	}
	printf("\nERRO: [Linha %d] R�tulo %s n�o encontrado!", linha, nome);
	return -1;
}

int transforma_opcode(char aux[], char opcode[], rotulos rot[], int nr, int linha) {
	char *token, mnemonico[5], op1[15], op2[15];
	int i, j, valor;
	
	fflush(stdin);
	token = strtok(aux, " \n"); 
	
	if(aux[0] != ' ') { /*Se possui um r�tulo no in�cio, ignora-o*/
		token = strtok(NULL, " \n");
	}

	
	if (token == NULL) return -1; /*Se n�o h� mnem�nico na linha, sai da fun��o*/
	strcpy(mnemonico, token);
	if (strcmp(mnemonico,"EQU")==0) return -1; /*Se for EQU, sai da fun��o*/
	
	//printf("\nMNEM�NICO: %s", mnemonico);
	
	// se for RET OU HLT, passar o codigo p/ o opcode e dar return
	if(strcmp(mnemonico, "RET") == 0) {
		strcpy(opcode, "c3h");
		return -2;
	}
	else if(strcmp(mnemonico, "HLT") == 0) {
		strcpy(opcode, "f4h");
		return -2;
	}
	
	
	
		token = strtok(NULL, ", \n"); /*Passa para o primeiro operador*/
		if (token == NULL) return -1;
		strcpy(op1, token);		
		
		if((strcmp(token, "[A]") != 0) && (strcmp(token, "[B]") != 0) && (strcmp(token, "A") != 0) && (strcmp(token, "B") != 0) ) { /*Se o operador n�o � [A] ou [B]*/
			//verificar se � n�mero ou r�tulo
			if (isdigit(token[0]) != 0){ //� n�mero
				valor = atoi(token);
			}
			else {
				if(strchr(token, '[') != NULL) { /*Se o operador tem colchetes, retira-os*/
				
					for(i = 0; i < strlen(token); i++){
						if (token[i] == '[' || token[i] == ']') {
							for(j = i; j < strlen(token); j++){
								token[j] = token[j+1];
							}
							i--;
						}	
					}				
				}
				valor = procura_rotulo(rot, token, nr, linha);
			}
		}
		
	//printf("\nvalor = %d", valor);
	if(strcmp(mnemonico, "JC") == 0) {
		strcpy(opcode, "72h");
		return valor;
	}
	else if(strcmp(mnemonico, "JNC") == 0) {
		strcpy(opcode, "73h");
		return valor;
	}
	else if(strcmp(mnemonico, "JZ") == 0) {
		strcpy(opcode, "74h");
		return valor;
	}
	else if(strcmp(mnemonico, "JNZ") == 0) {
		strcpy(opcode, "75h");
		return valor;
	}
	else if(strcmp(mnemonico, "JBE") == 0) {
		strcpy(opcode, "76h");
		return valor;
	}
	else if(strcmp(mnemonico, "JA") == 0) {
		strcpy(opcode, "77h");
		return valor;
	}
	else if(strcmp(mnemonico, "JMP") == 0) {
		strcpy(opcode, "ebh");
		return valor;
	}
	else if(strcmp(mnemonico, "CALL") == 0) {
		strcpy(opcode, "e8h");
		return valor;
	}
			
			
		//repete as a��es para pegar o op2
		token = strtok(NULL, ", \n");
		if (token == NULL) return 256;
		strcpy(op2, token);		
		
		if((strcmp(token, "[A]") != 0) && (strcmp(token, "[B]") != 0) && (strcmp(token, "A") != 0) && (strcmp(token, "B") != 0) ) { /*Se o operador n�o � [A] ou [B]*/
			//verificar se � n�mero ou r�tulo
			if (isdigit(token[0]) != 0){ //� numero 
				valor = atoi(token);
			}
			else {
				if(strchr(token, '[') != NULL) { /*Se o operador tem colchetes, retira-os*/
				
					for(i = 0; i < strlen(token); i++){
						if (token[i] == '[' || token[i] == ']') {
							for(j = i; j < strlen(token); j++){
								token[j] = token[j+1];
							}
							i--;
						}	
					}
									
				}
				valor = procura_rotulo(rot, token, nr, linha);
			}
		}
	
	
	/*Compara mnem�nicos com argumentos e retorna o opcode certo*/	
	
	if(strcmp(mnemonico, "ADD") == 0) {
		if(strchr(op2, '[') != NULL && strcmp(op2, "[B]") != 0) {
			strcpy(opcode, "02h");
		}
		else if(strcmp(op2, "[B]") == 0) {
			strcpy(opcode, "03h");
		}
		else if(strchr(op2, '[') == NULL) {
			strcpy(opcode, "04h");
		}
	}
	else if(strcmp(mnemonico, "SUB") == 0) {
		if(strchr(op2, '[') != NULL && strcmp(op2, "[B]") != 0) {
			strcpy(opcode, "2ah");
		}
		else if(strcmp(op2, "[B]") == 0) {
			strcpy(opcode, "2bh");
		}
		else if(strchr(op2, '[') == NULL) {
			strcpy(opcode, "2ch");
		}
	}
	else if(strcmp(mnemonico, "CMP") == 0) {
		if(strchr(op2, '[') != NULL && strcmp(op2, "[B]") != 0) {
			strcpy(opcode, "3ah");
		}
		else if(strcmp(op2, "[B]") == 0) {
			strcpy(opcode, "3bh");
		}
		else if(strchr(op2, '[') == NULL) {
			strcpy(opcode, "3ch");
		}
	}
	else if(strcmp(mnemonico, "INC") == 0) {
		if(strcmp(op1, "A") == 0 && strcmp(op2, NULL) == 0) {
			strcpy(opcode, "40h");
		}
		else if(strcmp(op1, "B") == 0 && strcmp(op2, NULL) == 0) {
			strcpy(opcode, "41h");
		}
	}
	else if(strcmp(mnemonico, "DEC") == 0) {
		if(strcmp(op1, "A") == 0 && strcmp(op2, NULL) == 0) {
			strcpy(opcode, "42h");
		}
		else if(strcmp(op1, "B") == 0 && strcmp(op2, NULL) == 0) {
			strcpy(opcode, "43h");
		}
	}
	else if(strcmp(mnemonico, "MOV") == 0){ 
		if(strcmp(op1, "A") == 0 && isdigit(op2[0]) != 0) {
			strcpy(opcode, "b0h");
		}
		else if(strcmp(op1, "A") == 0 && strcmp(op2, "B") == 0) {
			strcpy(opcode, "88h");
		}
		else if(strcmp(op1, "B") == 0 && strcmp(op2, "A") == 0) {
			strcpy(opcode, "8ah");
		}
		else if((strchr(op2, '[') != NULL) && (strcmp(op2, "[B]") != 0)) {
			strcpy(opcode, "a0h");
		}
		else if(strcmp(op2, "[B]") == 0) {
			strcpy(opcode, "a1h");
		}
		else if((strchr(op1, '[') != NULL) && (strcmp(op1, "[B]") != 0)) {
			strcpy(opcode, "a2h");
		}
		else if(strcmp(op1, "[B]") == 0) {
			strcpy(opcode, "a3h");
		}
	}
	else {
		printf("\nO comando %s � inv�lido.", mnemonico);
		getch();
		exit(1);
	}
	
	return valor;
}





//-----------------------------------------FUN��O PRINCIPAL-----------------------------------------------------------
int main () {
	
	setlocale(LC_ALL,"portuguese");
	
    //Vari�veis
    rotulos rot[101];
	int linha = 0, mem = 0 /*Conta os bytes usandos nos comandos*/, nr = 0 /*Contador de r�tulos*/, valor /*Valor do r�tulo*/;
    char aux[101] /*Armazena a linha de c�digo a ser lida*/, aux2[101] /*Auxiliar da auxiliar para strtok*/;
    char opcode[5]; /*Para a fun��o transforma_opcode*/
	int i; 
	char arquivo_entrada[31];
	
	printf("Digite o nome do arquivo de entrada (exemplo: \"entrada.txt\"): ");
	fflush(stdin);
	gets(arquivo_entrada);
	
    //Verifica��o dos arquivos
    if((entrada = fopen(arquivo_entrada, "rt")) == NULL){
        printf("\nO arquivo %s n�o existe.", arquivo_entrada); 
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
	linha = 0;
	rewind(entrada); /*Ler novamente o arquivo*/
	while(fgets(aux, 101, entrada) != NULL){
		remove_comentario(aux);
		
		if(strlen(aux) != 0 && aux[0] != '\n'){
			strcpy(aux2, aux);
			valor = transforma_opcode(aux2, opcode, rot, nr, linha);
			if (valor > -1) {
				// printa opcode no arquivo e verifica se valor != -1, se for, printa tbm em outra linha
				fflush(stdin);
				//printf("\nOPCODE: %s", opcode);
				
				fseek(saida, 0, SEEK_END);
				fprintf(saida, "%s\n", opcode);
				
				if (valor != -2) {
					//printf("\nARGUMENTO: %d", valor);
					//printf("\n\n");
					fseek(saida, 0, SEEK_END);
					fprintf(saida, "%d\n", valor);
				}	
			}
		}
		linha++;
	}
	fclose(entrada);
	fclose(saida); 
}
