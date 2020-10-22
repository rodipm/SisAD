// secoes paralelas
//uma das secoes contem uma regiao paralela: a regiao paralela é executada por 2 threads. 

#include <stdio.h>
#include <omp.h>

int i,j,k;
int main()
{ 
	printf("INICIO\n");
	omp_set_nested(1);  //permite aninhamento de paralelismo

	#pragma omp parallel for num_threads(2) private(i)
	for(i = 0; i < 10; i++)
	{ 
		printf("for 1 i=%d idthread=%d\n",i,omp_get_thread_num());
		fflush(stdout);
    
		#pragma omp parallel for num_threads(2) private(j)
		for(j = 0; j < 10; j++)
		{
			printf("for 2 i=%d j=%d idthread=%d\n",i,j,omp_get_thread_num());
			fflush(stdout);
        
			#pragma omp parallel for num_threads(2) private(k)
			for(k = 0; k < 10; k++)
			{
				printf("for 3 i=%d j=%d k=%d idthread=%d\n",i,j,k,omp_get_thread_num());
				fflush(stdout);
			} 
		} 
	}
  printf("FIM\n");
}