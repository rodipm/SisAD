#include <stdio.h>
#include <omp.h>

int i;
int A;
omp_lock_t sem;

int main()
{ 
	printf("INICIO\n");
	A = 0;

	omp_init_lock(&sem);
	omp_set_num_threads(2);

	#pragma omp parallel sections
	{
   		#pragma omp section
   		{ 
			printf("secao 1\n");
			omp_set_lock(&sem);
			A++;
			omp_unset_lock(&sem); 

			for (i = 0; i < 200000; i++){
				printf("a");
				fflush(stdout);
			}
		}

  		#pragma omp section
		{ 
			printf("secao 2\n");
			omp_set_lock(&sem);
			A++;
			omp_unset_lock(&sem); 

			for (i = 0; i < 200000; i++){
				printf("b");
				fflush(stdout);
			}
		}
	}

	printf ("A=%d\n",A);
	omp_destroy_lock(&sem);
	printf("FIM\n");
}