// Soma de Elementos de um vetor usando MPI_Send e MPI_Gather.
// Numero de elementos do vetor: multiplo do número de processos.

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int	n,n_nos, rank;
	int vetor[100000],soma_parcial,i,k;
	int soma_total,soma[4];
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &n_nos);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        
	n = 100000;
	k = n/n_nos;

//================= Divisao do vetor para todos os ranks ======================
	if(rank == 0){
		for(i = 0; i < n; i++)
			vetor[i] = 1;

		for(i = 1; i < n_nos; i++)
             MPI_Send(&vetor[k*i],k,MPI_INT,i,10,MPI_COMM_WORLD);
	}
     
	else
		MPI_Recv(vetor,k,MPI_INT,0,10,MPI_COMM_WORLD,&status);
//==============================================================================

//============== Calculo da soma parcial em cada rank =========================
	soma_parcial = 0;
        		
	for(i = 0; i < k; i++)
		soma_parcial+=vetor[i];
//=============================================================================        

//================= Recepcao das somas parciais no rank 0 ===================== 

	// Todos os ranks executam a chamada gather e o rank 0 recebe todas as somas parciais
	// OBS: Broadcast nao possui TAG mas possui um buffer (soma no caso) para receber os valores
	MPI_Gather(&soma_parcial,1,MPI_INT,soma,1,MPI_INT,0,MPI_COMM_WORLD);
    
    soma_total = 0;

	if(rank == 0){
		for(i = 0; i < n_nos; i++){
			printf("soma[%d]=%d\n",i,soma[i]);
			fflush(stdout); 
			soma_total += soma[i];
		}

		printf("RESULTADO=%d\n",soma_total);
		fflush(stdout); 
	}
//==============================================================================

	MPI_Finalize();
	
	return 0;
}
