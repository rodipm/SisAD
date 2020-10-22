//uso de semaforos do POSIX
#include <stdio.h>
#include <omp.h>
#include <semaphore.h>
int A,B,X,Y;
sem_t sem;

int main()
{ 
	printf("INICIO\n");

	sem_init(&sem,0,0);   // parametro2: 0-thread  1-processo
                              // parametro3: valor ( inteiro >=0)
	omp_set_num_threads(2);

	#pragma omp parallel sections
	{
		#pragma omp section
		{
			int i;
			printf("secao 1\n");
    
			for (i = 0; i < 10000; i++){
				printf("a");
				fflush(stdout);
			}
     
			A = 9;
			sem_post(&sem);

			for (i = 0 ;i < 10; i++){
				printf("b");
				fflush(stdout);
			}

	     		B = 15;
			printf("B=%d\n",B);
			fflush(stdout);
			sem_post(&sem);
		}

		#pragma omp section 	
		{ 
			printf("secao 2\n");
			fflush(stdout);
			sem_wait(&sem);
			X = A + 1;
			sem_wait(&sem);
			printf("B=%d\n",B);
			fflush(stdout);
			Y = B + 1;
		}
	}
 
	sem_destroy(&sem);
	printf("X=%d    Y=%d \n",X,Y);  //resultado correto: X=10   Y=16
	fflush(stdout);
	printf("FIM\n");
}
