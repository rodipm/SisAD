// Soma de elementos de um vetor usando MPI_Send e MPI_Get_count.
//  Numero de elentos do vetor: multiplo de numero de processos

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int	n,n_nos, rank;
	int soma_parcial,i,k;
	int soma_total,soma;
	int *vetor; 
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &n_nos);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

//================= Divisao do vetor para todos os ranks ======================
	if(rank == 0){
		n = 500;
		vetor = calloc(n,sizeof(int));
          
		for(i = 0; i < n; i++)
			vetor[i] = 1;
          
		k = n/n_nos;
          
		for(i = 1; i < n_nos; i++)
			MPI_Send(&vetor[0]+k*i,k,MPI_INT,i,10,MPI_COMM_WORLD);
	}

	else{
		// A combinacao das chamadas Probe e Get_count permite o receptor saber o tamanho exato da mensagem
		MPI_Probe(0,10,MPI_COMM_WORLD,&status);
		MPI_Get_count(&status,MPI_INT,&k);
        
		vetor = calloc(k,sizeof(int));
		MPI_Recv(vetor,k,MPI_INT,0,10,MPI_COMM_WORLD,&status);
        
		printf("k recebidos=%d\n",k);
		fflush(stdout);
	}
//============================================================================== 

//============== Calculo da soma parcial em cada rank =========================
	soma_parcial = 0;

	for(i = 0; i < k; i++)
		soma_parcial+=vetor[i];
        
	printf("soma_parcial=%d\n",soma_parcial);
	fflush(stdout);
//============================================================================= 

//================= Recepcao das somas parciais no rank 0 =====================  
	if(rank == 0){
		soma_total=soma_parcial;
           
		for(i = 1; i < n_nos; i++){
			MPI_Recv(&soma,1,MPI_INT,MPI_ANY_SOURCE,11,MPI_COMM_WORLD,&status);
			soma_total += soma;
		}

		printf("RESULTADO=%d\n",soma_total);
		fflush(stdout); 
	}
        
	else
		MPI_Send(&soma_parcial,1,MPI_INT,0,11,MPI_COMM_WORLD);
//============================================================================= 

	MPI_Finalize();

	return 0;
}
