#include <stdio.h>
#include <omp.h>

int i;
int A;
omp_lock_t sem;

int main()
{ 
	printf("INICIO\n");

	omp_init_lock(&sem);
	omp_set_lock(&sem);
	omp_set_num_threads(2);

	#pragma omp parallel sections
	{
		#pragma omp section
		{ 
			printf("secao 1\n");

			for (i = 0; i < 1000; i++){
				printf("a");
				fflush(stdout);
			}

    		A = 9;
    		omp_unset_lock(&sem);

    		for (i = 0; i < 10; i++){
    			printf("x");
				fflush(stdout);
			}
  		}

  		#pragma omp section{ 
  			printf("secao 2\n");
  			omp_set_lock(&sem);
			printf("secao2 A=%d\n",A);    
			omp_unset_lock(&sem); 

			for (i = 0; i < 10; i++){
				printf("b");
				fflush(stdout);
			}
		}
	}
	omp_destroy_lock(&sem);
	printf("FIM\n");
}