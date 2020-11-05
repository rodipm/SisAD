#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

#define WORKTAG 1
#define DIETAG 2
#define NUM_WORK_REQS 10

void master();
void slave();
void (*tab_func[10])();
void func0();
void func1();
void func2();
void func3();
void func4();
void func5();
void func6();
void func7();
void func8();
void func9();
 
void func0()
{ 
  int result = 0;
  printf("func0\n");
  fflush(stdout);
  sleep(1);
  MPI_Send(&result,1,MPI_INT,0,0,MPI_COMM_WORLD); 
}

void func1()
{
  int result = 1;
  printf("func1\n");
  fflush(stdout);
  sleep(1);
  MPI_Send(&result,1,MPI_INT,0,0,MPI_COMM_WORLD);
}

void func2()
{
  int result = 2;
  printf("func2\n");
  fflush(stdout);
  sleep(1);
  MPI_Send(&result,1,MPI_INT,0,0,MPI_COMM_WORLD);
}

void func3()
{
  int result = 3;
  printf("func3\n");
  fflush(stdout);
  sleep(1);
  MPI_Send(&result,1,MPI_INT,0,0,MPI_COMM_WORLD);
}

void func4()
{
  int result = 4;
  printf("func4\n");
  fflush(stdout);
  sleep(1);
  MPI_Send(&result,1,MPI_INT,0,0,MPI_COMM_WORLD);
}

void func5()
{
  int result = 5;
  printf("func5\n");
  fflush(stdout);
  sleep(1);
  MPI_Send(&result,1,MPI_INT,0,0,MPI_COMM_WORLD);
}

void func6()
{
  int result = 6;
  printf("func6\n");
  fflush(stdout);
  sleep(1);
  MPI_Send(&result,1,MPI_INT,0,0,MPI_COMM_WORLD);
}

void func7()
{
  int result = 7;
  printf("func7\n");
  fflush(stdout);
  sleep(1);
  MPI_Send(&result,1,MPI_INT,0,0,MPI_COMM_WORLD);
}

void func8()
{
  int result = 8;
  printf("func8\n");
  fflush(stdout);
  sleep(1);
  MPI_Send(&result,1,MPI_INT,0,0,MPI_COMM_WORLD);
}

void func9()
{
	int result = 9;
	printf("func9\n");
	fflush(stdout);
	sleep(1);
	MPI_Send(&result,1,MPI_INT,0,0,MPI_COMM_WORLD);
}

int main(int argc, char *argv[])
{
	int myrank;
	MPI_Init(&argc,&argv);	
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

	if (myrank == 0)
  		master();
	else  
		slave();

	MPI_Finalize();

	return 0;
}

/*master: envia pedidos de trabalhos aos escravos e coleta resultados*/
void master()
{ 
	int ntasks,rank,work;
	int result;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&ntasks);

	work = NUM_WORK_REQS-1;

	tab_func[0] = func0;
	tab_func[1] = func1;
	tab_func[2] = func2;
	tab_func[3] = func3;
	tab_func[4] = func4;
	tab_func[5] = func5;
	tab_func[6] = func6;
	tab_func[7] = func7;
	tab_func[8] = func8;
	tab_func[9] = func9;

	printf("master\n");
	fflush(stdout);
	
	for(rank = 1; rank < ntasks; rank++){
		MPI_Send(&work,1,MPI_INT,rank,WORKTAG,MPI_COMM_WORLD);
		printf ("master work=%d\n",work);
		fflush(stdout);
		work--;
	}

	/*recebe um resultado de qualquer escravo e atribue um novo trabalho até execução de todos*/ 
	while(work >= 0) {
		MPI_Recv(&result,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		printf("result =%d\n",result);
		fflush(stdout);
		MPI_Send(&work,1,MPI_INT,status.MPI_SOURCE,WORKTAG,MPI_COMM_WORLD);
		work--;
	}

	/* Recebe resultados dos escravos sem atribuir novos trabalhos*/
	for(rank = 1; rank < ntasks; rank++){
		MPI_Recv(&result,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		printf("result =%d\n",result);
		fflush(stdout);
	}

	/* comunica aos escravos para exit */
	for(rank = 1; rank < ntasks; rank++)
		MPI_Send(0,0,MPI_INT,rank,DIETAG,MPI_COMM_WORLD);
}

/*slave */
void slave()
{  
	int work;
	MPI_Status status;
  
	tab_func[0]=func0;
	tab_func[1]=func1;
	tab_func[2]=func2;
	tab_func[3]=func3;
	tab_func[4]=func4;
	tab_func[5]=func5;
	tab_func[6]=func6;
	tab_func[7]=func7;
	tab_func[8]=func8;
	tab_func[9]=func9;

	printf("slave\n");
	fflush(stdout);
  
	for(;;) {
		MPI_Recv(&work,1,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		if (status.MPI_TAG==DIETAG)
			break;
  
		printf ("slave work=%d\n",work);
		fflush(stdout);
		(*tab_func[work])();
	}
}