#include <stdio.h>
#include <omp.h>

#define MAXRET  1000000

double total_pi = 0.0;

int main()
{
	int i;
	double x, largura, local_pi;
  
	largura = 1.0/MAXRET;
	local_pi = 0.0;

	printf("largura %f\n",largura);

	omp_set_num_threads(4);

	#pragma omp parallel shared(total_pi,largura) firstprivate(local_pi) private(i,x)
	{
		#pragma omp for 
 		for (i = 0; i < MAXRET; i++){ 
 			x = ((i-0.5) * largura);
    		local_pi = local_pi + (4.0 / ( 1.0 + x * x));
    	}

  		local_pi = local_pi * largura;
		printf("LOCAL-PI %f\n",local_pi);	

		#pragma omp critical 
  		{
  			total_pi = total_pi + local_pi;
  			printf("TOTAL-PI %f\n",total_pi);  
  		}
	}
}
