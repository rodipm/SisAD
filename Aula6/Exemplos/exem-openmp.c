#include <stdio.h>
#include <omp.h>

int a;

int main()
{
	int i;
	a = 15;  
  
	#pragma omp parallel num_threads(4) shared (a) private(i)
	{
		int k; 
		i = 2;
		printf("a=%d    i=%d \n",a,i);
		fflush(stdout);

		for (k = 0; k < 100; k++){
			i++;
			printf("i=%d\n",i);
			fflush(stdout);
		} 	
	}

	printf("FIM\n");
}