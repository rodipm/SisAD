#include <stdio.h>
#include <sys/time.h>
#include "mede_time.h"

int i,j;
int tam[50]; 
int tam_matrizes;
int A[1200][1200],B[1200][1200],C[1200][1200];
int max,max_final;

void main()
{
  FILE *f;
  int i,j,k,iter,tamanho_matriz;
  int soma,soma_total; 
  
  f = fopen("tamanho", "r");
  fread(tam,4,50,f);
  
  max_final=0;
  TIMER_CLEAR;
  TIMER_START;
  
  for(iter = 0; iter < 50; iter++){
    tamanho_matriz = tam[iter];

    // Processando computacao heterogenea, dependendo de n
    // Inicializando matrizes A, B e C tamanho n
    for(i = 0; i < tamanho_matriz; i++)
      for(j = 0; j < tamanho_matriz; j++){
        A[i][j] = 1;
        B[i][j] = 1;
        C[i][j] = 0;
      }

    // C = A X B  tamanho n  
    for(i = 0; i < tamanho_matriz; i++)
      for(k = 0; k < tamanho_matriz; k++)
        for(j = 0; j < tamanho_matriz; j++)
          C[i][j] = C[i][j] + A[i][k]*B[k][j];

    // Soma dos elementos da matriz C
    max = 0;

    for(i = 0; i < tamanho_matriz; i++)
      for(j = 0; j < tamanho_matriz; j++)
        if(max < C[i][j]) 
          max = C[i][j];

    if(max_final < max) 
      max_final = max;       
    }

  TIMER_STOP;
  printf ("TEMPO : %12.7f\n",TIMER_ELAPSED);

  fclose(f);
  printf("MAXIMO FINAL=%d\n",max_final);
}