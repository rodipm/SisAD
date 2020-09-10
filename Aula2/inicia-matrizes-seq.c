#include <stdio.h>
#include <stdlib.h>
#include "mede_time.h"

double a[2000][2000], b[2000][2000], c[2000][2000];
int size;
inicia_matrizes()
{
  int i, j, k;
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

main(argc, argv)
int argc;
char **argv;
{  

  if (argc > 1){
    size = atoi(argv[1]);
    printf("Matriz %dx%d\n",size,size);
  }

  else {
    printf("Tamanho da matriz: ");
    scanf("%d",&size);
  }
  TIMER_CLEAR;
  TIMER_START;
  inicia_matrizes();
  TIMER_STOP;
  printf ("TEMPO INICIA [SIZE %d]: %12.7f\n",size,TIMER_ELAPSED);
  printf("a[0][0]=%f   a[15][20]=%f  b[15][20]=%f  b[%d][%d]=%f \n",a[0][0],a[15][20],size-1,size-1,b[15][20],b[size-1][size-1]);
}
