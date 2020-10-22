#include <stdio.h>
#include <omp.h>

int y[2];
int x[2];
int r[2];

int some_calculation (int id)
{ 
	int i,a;
	a = y[id];
	for (i = 0; i < 1000; i++)
		a = a + 1;
	return a;
}

int main()
{
	y[0] = 0;
	y[1] = 500; 
	omp_set_num_threads(2);

	#pragma omp parallel 		
	{
		int mytid = omp_get_thread_num();
		x[mytid] = some_calculation(mytid);

		#pragma omp barrier
		r[mytid] = mytid + x[0] + x[1];
	}

	printf("r[0]=%d\n",r[0]);
	printf("r[1]=%d\n",r[1]);
}