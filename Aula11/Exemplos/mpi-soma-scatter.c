// Soma dos elementos de uma matriz usando MPI_Scatter para distribuir os dados.
// Numero de elementos do vetor: multiplo do numero de processos

#include <stdio.h>
#include <mpi.h>

double a[1000][1000];

int main(int argc, char *argv[])
{
	int	n,n_nos, rank;
	int i,j,k;
	double soma,soma_final,parcial;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &n_nos);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        
	n = 1000;

	printf("INICIO\n");
	fflush(stdout);

	k = n/n_nos;

	if (rank == 0){
		for(i = 0; i < n; i++)
			for(j = 0; j < n; j++)
				a[i][j] = 1;
	}              

//================= Divisao do vetor para todos os ranks ======================
	// Todos os ranks executam a chamada Scatter e recebem parte da matriz do rank 0
	// OBS: Scatter nao possui TAG nem destino, apenas a fonte que no caso eh 0
	MPI_Scatter(a,k*n,MPI_DOUBLE,a,k*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
	printf("a[0][0]=%f\n",a[0][0]);
	fflush(stdout);
//==============================================================================

// ============== Calculo da soma parcial em cada rank =========================
	parcial = 0;
        
	for(i = 0; i < k; i++)
		for(j = 0; j < n; j++)
			parcial += a[i][j];

	printf("rank=%d soma_parcial=%f\n",rank,parcial);
	fflush(stdout);
//============================================================================= 

//================= Recepcao das somas parciais no rank 0 ===================== 
	if (rank == 0){
		soma_final = parcial;
		
		for(i = 1; i < n_nos; i++){
			MPI_Recv(&soma,1,MPI_DOUBLE,MPI_ANY_SOURCE,11,MPI_COMM_WORLD,&status);
			soma_final+=soma;
		}

		printf("RESULTADO=%f\n",soma_final);
		fflush(stdout); 
	}

	else
		MPI_Send(&parcial,1,MPI_DOUBLE,0,11,MPI_COMM_WORLD);
//============================================================================= 
             
	MPI_Finalize();
	return 0;
}