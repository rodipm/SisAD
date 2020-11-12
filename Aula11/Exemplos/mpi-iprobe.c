// Exemplo de uso do MPI_Iprobe. 
// Esta sendo simulado com sleep um retardamento na execucao do MPI_Send.
// Enquanto espera o receptor pode processar outras tarefas, simulado com sleep. 

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <mpi.h>

double a[1000][1000];

int main(int argc, char *argv[])
{
	int	n,n_nos, rank;
	int i,j;
	double soma,soma_sqrt,parcial_sqrt;
	int flag;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &n_nos);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	n = 1000;
        
	if(n_nos < 2){
		MPI_Finalize();
		return 0;
	}

	printf("INICIO\n");
	fflush(stdout);
     
//================= Divisao da matriz para todos os ranks ======================
	if(rank == 0){
		for(i = 0; i < n; i++)
			for(j = 0; j < n; j++)
				a[i][j] = 4;
              
          for (i=1;i<n_nos;i++){
          	printf("rank 0 antes de Send\n");
			fflush(stdout);
            sleep(10);
            MPI_Send(&a[0][0]+(n/n_nos*n*i),n/n_nos*n,MPI_DOUBLE,i,10,MPI_COMM_WORLD);
			printf("rank 0 apos Send\n");
			fflush(stdout); 
		}        
	}
     
	else{
		// A chamada IProbe seta a flag quando a mensagem com destino e TAG definidos for recebida
		MPI_Iprobe(0,10,MPI_COMM_WORLD,&flag,&status);
         
		while(flag == 0){
			printf("PROCESSAMENTO\n");
			fflush(stdout);
			sleep(1);
			MPI_Iprobe(0,10,MPI_COMM_WORLD,&flag,&status);
		}

        MPI_Recv(a,n/n_nos*n,MPI_DOUBLE,0,10,MPI_COMM_WORLD,&status);
	}
//==============================================================================

//============== Calculo da soma das raizes quadradas em cada rank =============   
	parcial_sqrt = 0;

	for(i = 0; i < n/n_nos; i++)
		for(j = 0; j < n; j++)
			parcial_sqrt += sqrt(a[i][j]);
        
	printf("rank=%d soma_parcial_sqrt=%f\n",rank,parcial_sqrt);
	fflush(stdout);
//==============================================================================

//================= Recepcao das somas parciais no rank 0 ===================== 
	if (rank == 0) {
		soma_sqrt=parcial_sqrt;
           
		for(i = 1; i < n_nos; i++){
			MPI_Recv(&soma,1,MPI_DOUBLE,MPI_ANY_SOURCE,11,MPI_COMM_WORLD,&status);
			soma_sqrt+=soma;
		}
			
		printf("RESULTADO=%f\n",soma_sqrt);
		fflush(stdout); 
	}
        
	else
		MPI_Send(&parcial_sqrt,1,MPI_DOUBLE,0,11,MPI_COMM_WORLD);
//============================================================================= 

	MPI_Finalize();

	return 0;
}