#include <stdio.h>
#include <omp.h>

int main()
{
	printf("INICIO\n");
	omp_set_num_threads(2);

#pragma omp parallel
	{
		int i;

		printf("regiao paralela THREAD %d\n", omp_get_thread_num());
		fflush(stdout);

		#pragma omp sections
		{
			#pragma omp section
			{
				printf("secao 1 THREAD %d\n", omp_get_thread_num());
				fflush(stdout);

				for (i = 0; i < 20; i++)
				{
					printf("a");
					fflush(stdout);
				}
				printf("\n");
			}

			#pragma omp section
			{
				printf("secao 2 THREAD %d\n", omp_get_thread_num());
				fflush(stdout);

				for (i = 0; i < 20; i++)
				{
					printf("b");
					fflush(stdout);
				}
				printf("\n");
			}

			#pragma omp section
			{
				printf("secao 3 THREAD %d\n", omp_get_thread_num());
				fflush(stdout);

				for (i = 0; i < 20; i++)
				{
					printf("b");
					fflush(stdout);
				}
				printf("\n");
			}
		}
	}
	printf("FIM\n");
}
