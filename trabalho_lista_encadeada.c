#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <locale.h>

typedef struct reg *no;
struct reg {
	int info;
	struct reg *prox;
};

int main(){
	setlocale(LC_ALL, "Portuguese");
	char reproc; /*resposta do reprocessamento*/
	int op;
	no lista = NULL;
	
	do {
		printf("MENU INCIAL\n\n1- Admiss�o de funcion�rio novo\n2- Demiss�o de funcion�rio\n3- Mudan�a de departamento por parte do funcion�rio\n4- Rela��o dos funcion�rios de um departamento\n\nOp��o: ");
		scanf(" %d", &op);
		switch (op){
			case 1:
				//do...
				break;
			case 2:
				//do...
				break;
			case 3:
				//do...
				break;
			case 4:
				//do...
				break;
			default:
				printf("\nOp��o inv�lida.");
				break;
		}
		do {
			printf("\nIr novamente? S/N");
			reproc = toupper(getch());
			system("cls");
		} while (reproc != 'S' && reproc != 'N');
	} while (reproc == 'S');
}
