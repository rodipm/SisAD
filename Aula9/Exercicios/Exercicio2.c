/*
Obtém o numero de ocorrencias, em um vetor de inteiros, de um elemento definido pelo usuário
 */
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int n, n_nos, rank;
	MPI_Status status;
	int inicio, vetor[100000], i, k;
	int ocorrencias_parcial, ocorrencias_total, ocorrencias;

	int elemento;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &n_nos);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	n = 100000;
	k = n / n_nos;

	//================= Divisao do vetor para todos os ranks ======================
	if (rank == 0)
	{
		printf("Inteiro a ser buscado: ");
		fflush(stdout);
		scanf("%d", &elemento);

		// srand(time(NULL));
		for (int i = 0; i < n; i++)
			vetor[i] = i % 400;

		for (i = 1; i < n_nos; i++)
		{
			MPI_Send(&vetor[k * i], k, MPI_INT, i, 10, MPI_COMM_WORLD);
		}
	}

	else
	{
		MPI_Recv(vetor, k, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
	}
	//==============================================================================

	//=========== Calculo da ocorrencia parcial em cada rank =======================
	ocorrencias_parcial = 0;

	for (i = 0; i < k; i++) {
		int cur_val = vetor[i];
		if (cur_val == elemento)
			ocorrencias_parcial += 1;
	}

	printf("rank=%d ocorrencias_parcial=%d\n", rank, ocorrencias_parcial);
	fflush(stdout);

	//=============================================================================

	//============== Recepcao das ocorrencias parciais no rank 0 ==================
	if (rank == 0)
	{
		ocorrencias_total = ocorrencias_parcial;

		for (i = 1; i < n_nos; i++)
		{
			MPI_Recv(&ocorrencias, 1, MPI_INT, MPI_ANY_SOURCE, 11, MPI_COMM_WORLD, &status);
			ocorrencias_total += ocorrencias;
		}

		printf("RESULTADO=%d\n", ocorrencias_total);
		fflush(stdout);
	}

	else
		MPI_Send(&ocorrencias_parcial, 1, MPI_INT, 0, 11, MPI_COMM_WORLD);

	//===============================================================================
	MPI_Finalize();
	return 0;
}