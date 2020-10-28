/*
Obtém o valor máximo em um vetor de inteiros gerados aleatoriamente
 */
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int n, n_nos, rank;
	MPI_Status status;
	int vetor[100000], i, k;
	int max_val_parcial, max_val_total, max_val;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &n_nos);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	n = 100000;
	k = n / n_nos;

	//================= Divisao do vetor para todos os ranks ======================
	if (rank == 0)
	{
		srand(time(NULL));
		for (int i = 0; i < n; i++)
			vetor[i] = rand();

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

	//============= Calculo do max_val parcial em cada rank ========================
	max_val_parcial = -1;
	int cur_val;

	for (i = 0; i < k; i++) {
		cur_val = vetor[i];
		if (cur_val > max_val_parcial)
			max_val_parcial = cur_val;
	}

	printf("RANK[%d] - max_val_parcial=%d\n", rank, max_val_parcial);
	fflush(stdout);

	//=============================================================================

	//================ Recepcao dos max_val_parciais no rank 0 ====================
	if (rank == 0)
	{
		max_val_total = max_val_parcial;

		for (i = 1; i < n_nos; i++)
		{
			MPI_Recv(&max_val, 1, MPI_INT, MPI_ANY_SOURCE, 11, MPI_COMM_WORLD, &status);
			if (max_val > max_val_total)
				max_val_total = max_val;
		}

		printf("\nRESULTADO=%d\n", max_val_total);
		fflush(stdout);
	}

	else
		MPI_Send(&max_val_parcial, 1, MPI_INT, 0, 11, MPI_COMM_WORLD);

	//===============================================================================
	MPI_Finalize();
	return 0;
}