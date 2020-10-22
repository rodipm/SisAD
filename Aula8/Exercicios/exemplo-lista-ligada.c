#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mede_time.h"
#include <sys/time.h>
#define n 10

typedef struct {int a; int *proximo;} tipo_lista;

tipo_lista *lista;
tipo_lista *lista_first;

void inicia(int size)
{
	int i;
	tipo_lista *lista_anterior;
	lista = (tipo_lista *) malloc(sizeof(tipo_lista));
	lista->a = 0; 
	lista_first = lista; 

	for(i = 1; i < size; i++){ 
		lista_anterior = lista;
		lista = (tipo_lista *) malloc(sizeof(tipo_lista));
		lista->a = (i%5)+1000;
		lista_anterior->proximo = (int *)lista;
	}

	lista->proximo = (int *) 0; 
} 
     
void main()
{  
	int a[1000][1000];
	int b = 0;
	tipo_lista *prox;
	inicia(n); 
	prox = lista_first;
       
	TIMER_CLEAR;
	TIMER_START;    
	
	while(prox){
		lista = prox;
		printf("%d \n",lista->a);
		int i,j,x,k;
		x = 0;    //processando dado lista->a
                
		for(i = 1; i < 1000; i++)
			for(j = 0; j < 1000; j++)
				for(k = 0; k < lista->a; k++){
					x = x*2 + 1;
					a[i][j] = x + i*2 + j*3;
				}
                
		b = (b+a[999][999])%1000;
		//sleep(lista->a);
		prox = (tipo_lista*)lista->proximo;
	}
        
	TIMER_STOP;
	printf("TEMPO: %f\n",TIMER_ELAPSED);
	printf("b=%d\n",b);
	printf("FIM MAIN\n");
}