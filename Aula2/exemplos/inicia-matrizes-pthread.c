#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "mede_time.h"
#define n_threads 3

double a[2000][2000], b[2000][2000],c[2000][2000];
int n_iter;
int size;
pthread_t tid[n_threads];

void *inicia_matrizes(void *arg)
{  
  int i, j,k,inicio, fim, id_thread;

  id_thread = *(int *)arg;   
  inicio = (id_thread+1) * n_iter;
  fim = inicio + n_iter;
  //printf("inicio=%d  fim=%d   id_thread=%d   n_iter=%d\n",inicio,fim,id_thread,n_iter);

  for(i = inicio; i < fim; i++)
    for(j = 0; j < size; j++){
      a[i][j]=0;
      b[i][j]=0;
      for (k=0;  k<size;  k++){
        a[i][j] = a[i][j]+0.2*i+1.03*j+1.5*k;
        b[i][j] = b[i][j]+0.3*i+0.2*j+1.7*k;
      }
      c[i][j]=0;
   }
}

void main(argc, argv)
int argc;
char **argv;
{  
  int i, k, id_thread[n_threads];

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
  n_iter = size/(n_threads+1);

  //inicia as matrizes - 3 threads e principal
  for (k = 0; k < n_threads; k++){
    id_thread[k] = k;

    if(pthread_create(&tid[k],NULL,inicia_matrizes,(void *)&id_thread[k])){
      printf("Cannot create thread %d\n",i);
      exit(1);
    }
  }

  int p = -1;
  inicia_matrizes((void *)&p);
  /* join threads */
  for (i = 0; i < n_threads; i++){
        pthread_join(tid[i],NULL);
  }
  TIMER_STOP;
  printf ("TEMPO INICIA [SIZE %d]: %12.7f\n",size,TIMER_ELAPSED);
  printf("a[0][0]=%f   a[15][20]=%f  b[15][20]=%f  b[%d][%d]=%f \n",a[0][0],a[15][20],b[15][20],size-1,size-1,b[size-1][size-1]);
}
