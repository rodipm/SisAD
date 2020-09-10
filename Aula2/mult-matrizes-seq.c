/*multiplicacao de matrizes*/
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "mede_time.h"
#include <stdlib.h>
int size;

double a[2000][2000];
double b[2000][2000];
double c[2000][2000]; 

/*inicia matriz*/
inicia_matrizes()
{ 
  int i, j,k;

  for(i = 0; i < size; i++){  
    for (j = 0; j < size; j++){
      a[i][j]=0;
      b[i][j]=0;
      for (k=0;  k<size;  k++){
        a[i][j] = a[i][j]+0.2*i+1.03*j+1.5*k;
        b[i][j] = b[i][j]+0.3*i+0.2*j+1.7*k;
      }
      c[i][j]=0;
    }
  }
}


/*multiplicacao*/
mat_mult()
{
  int i, j, k;

  for(i = 0; i < size; i++){
    for(k = 0; k < size; k++) 
      for(j = 0; j < size; j++)
        c[i][j] = c[i][j] + a[i][k] * b[k][j];
  }
}

main(argc, argv)
int argc;
char **argv;
{

  if (argc > 1){ 
    size = atoi(argv[1]);
    printf("Matriz %dx%d\n",size,size);
  }

  else{
    printf("Tamanho da matriz: ");
    scanf("%d",&size);
  }
 
  TIMER_CLEAR;
  TIMER_START; 
  inicia_matrizes();
  TIMER_STOP;
  printf ("TEMPO INICIA [SIZE %d]: %12.7f\n",size,TIMER_ELAPSED);
  TIMER_CLEAR;
  TIMER_START; 
   mat_mult();
  TIMER_STOP;
  printf ("TEMPO MULTIPLICA [SIZE %d]: %12.7f\n",size,TIMER_ELAPSED);
  printf ("c[0][0]=%f  c[2][1]=%f c[%d][%d]=%f\n",c[0][0],c[2][1],size-1,size-1,c[size-1][size-1]);
}

