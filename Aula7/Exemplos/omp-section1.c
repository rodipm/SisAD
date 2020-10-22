#include <stdio.h>
#include <omp.h>

int i;

int main()
{ 
	printf("INICIO\n");
	omp_set_num_threads(2);

	#pragma omp parallel sections
	{
		#pragma omp section
		{ 
			printf("secao 1\n");
			for (i = 0; i < 20; i++){
				printf("a");
				fflush(stdout);
			}
		}

		#pragma omp section
		{ 
			printf("secao 2\n");
			for (i = 0; i < 20; i++){
				printf("b");
      			fflush(stdout);
			}
  		}
	}

  printf("FIM\n");
}

