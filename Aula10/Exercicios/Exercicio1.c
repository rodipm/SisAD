#include <stdio.h>
#include <sys/time.h>
#include "mede_time.h"
#include <mpi.h>

#define WORKTAG 1
#define DIETAG 2
#define NUM_ITERS 50

int tam[50];
int A[1200][1200], B[1200][1200], C[1200][1200];
int max_final;

void master();
void slave();

void computacao_heterogenea(int iter)
{
  int tamanho_matriz = tam[iter];
  printf("iter=%d\n", iter);
  printf("tamanho_matriz=%d\n", tamanho_matriz);

  // Processando computacao heterogenea, dependendo de n
  // Inicializando matrizes A, B e C tamanho n
  for (int i = 0; i < tamanho_matriz; i++)
    for (int j = 0; j < tamanho_matriz; j++)
    {
      A[i][j] = 1;
      B[i][j] = 1;
      C[i][j] = 0;
    }

  // C = A X B  tamanho n
  for (int i = 0; i < tamanho_matriz; i++)
    for (int k = 0; k < tamanho_matriz; k++)
      for (int j = 0; j < tamanho_matriz; j++)
        C[i][j] = C[i][j] + A[i][k] * B[k][j];

  // Soma dos elementos da matriz C
  int max = 0;

  for (int i = 0; i < tamanho_matriz; i++)
    for (int j = 0; j < tamanho_matriz; j++)
      if (max < C[i][j])
        max = C[i][j];

  MPI_Send(&max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
}

int max(int new_max)
{
  if (new_max > max_final)
    return new_max;
  return max_final;
}

void master()
{
  int nprocs, rank, iter_corrente;
  int result;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  max_final = -1;

  printf("master\n");
  fflush(stdout);

  for (rank = 1; rank < nprocs; rank++)
  {
    MPI_Send(&iter_corrente, 1, MPI_INT, rank, WORKTAG, MPI_COMM_WORLD);
    printf("master iter_corrente=%d\n", iter_corrente);
    fflush(stdout);
    iter_corrente++;
  }

  /*recebe um resultado de qualquer escravo e atribue um novo trabalho até execução de todos*/
  while (iter_corrente < NUM_ITERS)
  {
    MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    printf("result =%d\n", result);
    fflush(stdout);
    max_final = max(result);
    MPI_Send(&iter_corrente, 1, MPI_INT, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD);
    iter_corrente++;
  }

  /* Recebe resultados dos escravos sem atribuir novos trabalhos*/
  for (rank = 1; rank < nprocs; rank++)
  {
    MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    printf("result =%d\n", result);
    max_final = max(result);
    fflush(stdout);
  }

  /* comunica aos escravos para exit */
  for (rank = 1; rank < nprocs; rank++)
    MPI_Send(0, 0, MPI_INT, rank, DIETAG, MPI_COMM_WORLD);
}

void slave()
{
  int iter;
  MPI_Status status;

  printf("slave\n");
  fflush(stdout);

  for (;;)
  {
    MPI_Recv(&iter, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    if (status.MPI_TAG == DIETAG)
      break;

    printf("slave iter=%d\n", iter);
    fflush(stdout);
    computacao_heterogenea(iter);
  }
}

int main(int argc, char *argv[])
{
  int myrank;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  FILE *f;
  int i, j, k, iter, tamanho_matriz;
  int soma, soma_total;

  f = fopen("tamanho", "r");
  fread(tam, 4, 50, f);

  printf("Inicio\n");

  TIMER_CLEAR;
  TIMER_START;

  if (myrank == 0)
  {
    master();
    TIMER_STOP;

    printf("TEMPO : %12.7f\n", TIMER_ELAPSED);

    fclose(f);
    printf("MAXIMO FINAL=%d\n", max_final);
  }
  else
    slave();

  MPI_Finalize();
  return 0;
}