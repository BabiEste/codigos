//6) Demonstre como implementar um fila est�tica de inteiros em C, usando um vetor fila[100], onde
//fila[0] � usado para indicar o in�cio da fila, fila[1] para indicar o final da fila e as outras posi��es
//(fila[2] a fila[99]) guardam os elementos da fila. Demonstre como inicializar esse vetor de modo a
//representar a fila vazia. Escreva as rotinas desenfileira (.),enfileira(.), vazia(.) e cheia(.) para tal
//implementa��o.

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <locale.h>

#define MAX 100

//-----------------------------------------
//Structs e Unions
//-----------------------------------------

typedef int def_fila[MAX];

//-----------------------------------------
//Fun��es
//-----------------------------------------

int cheia(def_fila fila){
	return (fila[1] == MAX-1); //fila[1] = final
}

int vazia(def_fila fila){
	return (fila[0] == -1); //fila[0] = inicio
}

int enfileira(def_fila fila, int numero){
	int inicio = fila[0];
	int final = fila[1];
	
	if(cheia(fila)) 
		return 0;
	fila[++final] = numero;
	if(inicio = -1) //Se a fila era nula, inicializa-a.
		inicio = 0;
	fila[0] = inicio;
	fila[1] = final;
	return 1;
}

int desenfileira(def_fila fila, int *numero){
	int inicio = fila[0];
	int final = fila[1];
	if(vazia(fila))
		return 0;
	*numero = fila[inicio];
	if(inicio == final) //Se s� tinha um elemento na fila, zera a fila
		inicio = final = -1;
	else
		inicio++; //Se n�o pega o valor do pr�ximo
}

void mostra_fila(def_fila fila){
	int i, inicio, final;
	inicio = fila[0];
	final = fila[1];
	for(i = inicio; i <= final; i++){
		printf(" %d", fila[i]);
	}
}

void inicializa(def_fila fila){
	fila[0] = -1;
	fila[1] = -1;
}

int menu(){
	int op;
	printf("\n-----------------------------------------\n1- Adicionar n�mero na fila\n2- Remover n�mero da fila\nOp��o: ");
	scanf(" %d", &op);
	return op;
}

//-----------------------------------------
//Main
//-----------------------------------------
int main(){
	setlocale(LC_ALL, "portuguese");
	def_fila fila;
	int numero, op, opcao_menu;
	while(1){
		inicializa(fila);
		opcao_menu = menu();
		switch(opcao_menu){
			case 1:
				do {
					printf("\nDigite um n�mero: ");
					scanf(" %d", &numero);
					if(!enfileira(fila, numero)) {
						printf("\nN�o foi poss�vel adicionar um valor � fila.\n");
						op = -1;
					}	
					else {
						printf("\nFila: ");
						mostra_fila(fila);
						printf("\nAdicionar outro n�mero (-1 para cancelar)? ");
						scanf(" %d", &op);
					}	
				} while (op != -1);
				break;
			case 2:
				if(!desenfileira(fila, &numero))
					printf("\nA fila j� est� vazia.\n");
				else {
					printf("\nN�mero retirado: %d\nFila restante: ", numero);
					mostra_fila(fila);
				}
		}
		
	}
}

