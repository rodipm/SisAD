/*
    Busca a primeira ocorrência, em 4 vetores de inteiros, de um elemento definido pelo usuário
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define SIZE 100000

int *A, *B, *C, *D;

void inicializa_vetor(int **V, int rank)
{
    srand(time(NULL) + 10*rank);
    for (int i = 0; i < SIZE; i++)
    {
        (*V)[i] = rand() % 20;
    }
}

int obtem_primeira_ocorrencia(int elemento, int **V){
    for (int i = 0; i < SIZE; i++) {
        if ((*V)[i] == elemento)
            return i;
    }
    return -1;
}

int main(int argc, char *argv[])
{
    int rank;
    int inicio, i, k;
    int vetor_ocorrencias[4], primeira_ocorrencia;

    int elemento;

    A = malloc(SIZE * sizeof(int));
    B = malloc(SIZE * sizeof(int));
    C = malloc(SIZE * sizeof(int));
    D = malloc(SIZE * sizeof(int));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //========== Rank 0 obtém o elemento a ser buscado para todos os nós ==========
    if (rank == 0)
    {
        printf("Inteiro a ser buscado: ");
        fflush(stdout);
        scanf("%d", &elemento);
    }

    // Broadcast do elemento para todos os ranks
    // MPI_Bcast(void *buf, int count, MPI_Datatype dtype, int root,MPI_Comm comm);
    MPI_Bcast(&elemento, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("elemento[%d]=%d\n", rank, elemento);
    //=========== Inicialização dos vetores em cada rank =======================

    int **v = NULL;
    if (rank == 0)
        v = &A;
    if (rank == 1)
        v = &B;
    if (rank == 2)
        v = &C;
    if (rank == 3)
        v = &D;

    inicializa_vetor(v, rank);

    //=========== Calculo da ocorrencia parcial em cada rank =======================
    primeira_ocorrencia = obtem_primeira_ocorrencia(elemento, v);

    printf("primeira_ocorrencia[%d]=%d\n", rank, primeira_ocorrencia);
    fflush(stdout);


    //============== Recepcao das primeiras ocorrencias rank 0 ==================

    // MPI_Gather (void *sendbuf, int sendcount,MPI_Datatype sendtype, void *recvbuf,int recvcount,MPI_Datatype recvtype,int root, MPI_Comm comm);
    MPI_Gather(&primeira_ocorrencia, 1, MPI_INT, vetor_ocorrencias, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (i = 0; i < 4; i++)
        {
            printf("PRIMEIRA_OCORRENCIA[%d]=%d\n", i, vetor_ocorrencias[i]);
        }
    }
    //===============================================================================
    MPI_Finalize();
    return 0;
}