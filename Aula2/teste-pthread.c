// o valor de i é passado como um elemento de um vetor : arg_i[i] 
// arg_i:  ponteiro 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_t *tid;

void *teste(void *arg)
{
  int i;
  char carac;
  carac = *(char*) arg;
  for (i = 0; i < 100000; i++){
    printf("%c",carac);
    fflush(stdout);
  }
}

void main(argc,argv)
int argc;
{
  int i;
  char x[i];
  tid = (pthread_t *) calloc(3, sizeof(pthread_t));

/* create threads */
  x[0] = 'a';
  i = 0;

  if (pthread_create(&tid[i], NULL, teste, (void *)&x[0])){
    printf("Cannot create thread %d\n",i);
    exit(0);
  }

  x[1] = 'b';
  i = 1;

  if (pthread_create(&tid[i], NULL, teste, (void *)&x[1])){
    printf("Cannot create thread %d\n",i);
    exit(0);
  }

  x[2] = 'c';
  i = 2;

  if (pthread_create(&tid[i], NULL, teste, (void *)&x[2])){
    printf("Cannot create thread %d\n",i);
    exit(1);
  }   

  /* join threads */
  for (i = 0; i < 3; i++)
    pthread_join(tid[i], NULL);

  printf("FIM\n");
}
