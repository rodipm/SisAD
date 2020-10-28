/*
Soma de Elementos de um vetor usando MPI_Send.
Numero de elementos do vetor: multiplo do número de processos.
 */
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int	n,n_nos, rank;
	MPI_Status	status;
	int inicio,vetor[100000],i,k;
	int soma_parcial,soma_total,soma;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &n_nos);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        
	n = 100000;
	k = n / n_nos;
	inicio = rank * k;

//================= Divisao do vetor para todos os ranks ======================
	if (rank == 0){
		for(i = inicio; i < n; i++)
			vetor[i] = 1;

		for (i = 1; i < n_nos; i++){
			MPI_Send(&vetor[k*i],k,MPI_INT,i,10,MPI_COMM_WORLD);
			printf("rank=%d APOS SEND\n",rank);
			fflush(stdout);
		}
	}

	else {
		MPI_Recv(vetor,k,MPI_INT,0,10,MPI_COMM_WORLD,&status);
		printf("rank=%d APOS RECV\n",rank);
		fflush(stdout);
	}
//==============================================================================

//============== Calculo da soma parcial em cada rank =========================
	soma_parcial = 0;

	for(i = 0; i < k; i++)
		soma_parcial += vetor[i];

	printf("rank=%d soma_parcial=%d\n",rank,soma_parcial);
	fflush(stdout);

//=============================================================================


//================= Recepcao das somas parciais no rank 0 =====================
	if (rank == 0){
		soma_total = soma_parcial;
          
		for(i = 1; i < n_nos; i++){
			MPI_Recv(&soma,1,MPI_INT,MPI_ANY_SOURCE,11,MPI_COMM_WORLD,&status);
			soma_total += soma;
		}
           
		printf("RESULTADO=%d\n",soma_total);
		fflush(stdout); 
	}

	else
		MPI_Send(&soma_parcial,1,MPI_INT,0,11,MPI_COMM_WORLD);

//===============================================================================
	MPI_Finalize();
	return 0;
}