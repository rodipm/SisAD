// task com regiao paralela
// 2 threads para executar regiao paralela
#include <stdio.h>
// #include <unistd.h>
#include <omp.h>

int main()
{
	// 	float a[1000][1000];
	// 	float b[1000][1000];

	printf("INICIO\n");
	fflush(stdout);
#pragma omp parallel num_threads(2)
	{

#pragma omp single
		{
			printf("thread %d\n", omp_get_thread_num());
			fflush(stdout);
			printf(" EXEMPLO \n");
			fflush(stdout);
#pragma omp task
			{
				int i, j, x, k;
				x = 0;
				for (i = 0; i < 1000; i++)
					for (j = 0; j < 1000; j++)
						for (k = 0; k < 100; k++)
						{
							x = x * 2 + 1;
							a[i][j] = x + i * 2.45 + j * 3.78;
						}
				printf(" DE thread %d a[900][900]=%f\n", omp_get_thread_num(), a[900][900]);
				fflush(stdout);
			}

#pragma omp task
			{
				int i, j, x, k;
				x = 0;
				for (i = 0; i < 1000; i++)
					for (j = 0; j < 1000; j++)
						for (k = 0; k < 1000; k++)
						{
							x = x * 3 + 1;
							b[i][j] = x + i * 1.45 + j * 2.71;
						}
				printf(" TASK thread %d b[900][900]=%f\n", omp_get_thread_num(), b[900][900]);
				fflush(stdout);
			}
			printf("APOS criar TASKS\n");
			fflush(stdout);
		}
	}
	printf("FIM MAIN \n");
	fflush(stdout);
}
