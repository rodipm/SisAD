//INICIACAO DAS MATRIZES a,b e c  
//TAMANHO ATE 2000 X 2000
//execucao 1000 X 1000:    inicia-matrizes-proc   1000  

#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include "mede_time.h"

#define n_processos 3

typedef struct {
   double a[2000][2000], b[2000][2000], c[2000][2000];
} matrizes;
// matriz
matrizes *matriz;
pid_t pid_p;
int size;
int n_iter;
inicia_matrizes(int id_proc)
{
  int i, j,k;
  int inicio_linhas,fim_linhas;

  inicio_linhas = (id_proc+1) * n_iter;
  fim_linhas = inicio_linhas + n_iter;
  printf("inicio_linhas=%d    fim_linhas=%d\n",inicio_linhas,fim_linhas);
  fflush(stdout);
  for(i = inicio_linhas; i < fim_linhas; i++){
    for (j = 0; j < size; j++){
      ((matrizes *)matriz)->a[i][j]=0;
      ((matrizes *)matriz)->b[i][j]=0;
      for (k=0;  k<size;  k++){
        ((matrizes *)matriz)->a[i][j] = ((matrizes *)matriz)->a[i][j]+0.2*i+1.03*j+1.5*k;
        ((matrizes *)matriz)->b[i][j] = ((matrizes *)matriz)->b[i][j]+0.3*i+0.2*j+1.7*k;
      }
      ((matrizes *)matriz)->c[i][j]=0;
    }
  }
}

main(argc, argv)
int argc;
char **argv;
{  
  int i,j;
  pid_t pid[n_processos];
  int status;
  struct shmid_ds *buf;
  int id_matriz;
  int inicio_linhas,fim_linhas;
  int id_proc;

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
  id_matriz = shmget(IPC_PRIVATE, sizeof(matrizes), 0777 | IPC_CREAT);
  matriz = shmat(id_matriz, 0, 0);

  //processo principal inicializa as matrizes   -  3 processos
  n_iter = size/(n_processos + 1);
  id_proc=-1;
//processo principal cria processos e armazena pid retornado em vetor
  pid_p=1;
  for (i=0;((i<n_processos) && (pid_p!=0));i++) {     
    pid_p = fork();
    if ((int)pid_p != 0)   //pai
      pid[i] = pid_p;
    else {    // filhos
      matriz = shmat(id_matriz, 0, 0);
      id_proc=i;
    }
  } 
  printf("id_proc=%d     n_iter=%d\n",id_proc,n_iter);
  fflush(stdout);
  inicia_matrizes(id_proc);
  if ((int)pid_p == 0){     //filhos
    shmdt(matriz);
    exit(0);
  }

  // fim da inicializacao
  for(i = 0; i < n_processos; i++) 
    waitpid(pid[i],&status,0);
  TIMER_STOP;
  printf ("TEMPO INICIA [SIZE %d]: %12.7f\n",size,TIMER_ELAPSED);
  printf("a[0][0]=%f   a[15][20]=%f  b[15][20]=%f  b[%d][%d]=%f \n",((matrizes *)matriz)->a[0][0],((matrizes *)matriz)->a[15][20],size-1,size-1,((matrizes *)matriz)->b[15][20],((matrizes *)matriz)->b[size-1][size-1]);
  shmdt(matriz);
  shmctl(id_matriz, IPC_RMID, buf); //remove area compartilhada
}
