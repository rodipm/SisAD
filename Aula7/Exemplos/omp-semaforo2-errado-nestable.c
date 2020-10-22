//programama usando nestable lock
// ERRADO: em algumas execuções os valores de B na secao 2 estão incorretos (0),
// mas o valor de Y resulta correto (no momento do calculo de Y o valor de B já estava atualizao. Em algumas execucoes valores de Y são incorretos.
// Race condtion.
// ERRO: unlock e lock efetuadas em regióes paralelas diferentes
// até OpenMP 2.5 se o owner fosse a mesma thread, embora em regiões paralelas diferentes, como neste exemplo, estaria correto. Nas versoes a partir de 3.0 passou a ser incorreto se efetuadas em regioes distintas.
   
#include <stdio.h>
#include <omp.h>

int A,B,X,Y;
omp_nest_lock_t sem;

int main()
{ 
	printf("INICIO\n");

	omp_init_nest_lock(&sem);
	omp_set_nest_lock(&sem);
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
			omp_unset_nest_lock(&sem);

			for (i = 0 ;i < 10; i++){
				printf("b");
				fflush(stdout);
			}

	     		B = 15;
			printf("B=%d\n",B);
			fflush(stdout);
			omp_unset_nest_lock(&sem);
		}

		#pragma omp section 	
		{ 
			printf("secao 2\n");
			fflush(stdout);
			omp_set_nest_lock(&sem);
			X = A + 1;
			omp_set_nest_lock(&sem);
			printf("B=%d\n",B);
			fflush(stdout);
			Y = B + 1;
		}
	}
 
	omp_destroy_nest_lock(&sem);
	printf("X=%d    Y=%d \n",X,Y);  //resultado correto: X=10   Y=16
	fflush(stdout);
	printf("FIM\n");
}
