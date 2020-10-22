#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int i;
int n;
int v[1000];

void inicia_vetor(int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		v[i] = random() % 1000;
	}
}

void intercala(int p, int q, int r, int v[])
{
	int i, j, k;
	int v_aux[1000];
	i = p;
	j = q;
	k = 0;

	while (i < q && j < r)
	{
		if (v[i] <= v[j])
			v_aux[k++] = v[i++];
		else
			v_aux[k++] = v[j++];
	}

	while (i < q)
	{
		v_aux[k++] = v[i++];
	}

	while (j < r)
	{
		v_aux[k++] = v[j++];
	}

	for (i = p; i < r; i++)
		v[i] = v_aux[i - p];
}

void merge_sort(int p, int r, int v[])
{
	int q;
	if (p < r - 1)
	{
		q = (p + r) / 2;
		printf("p=%d, r=%d, q=%d\n, thread=%d", p, r, q, omp_get_thread_num());
		#pragma omp task 
		{
			merge_sort(p, q, v);
		}
		#pragma omp task 
		{
			merge_sort(q, r, v);
		}
		
		#pragma omp taskwait
		intercala(p, q, r, v);
	}
}

void main()
{
	int i;
	printf("n:");
	scanf("%d", &n);
	printf("n=%d\n", n);
	inicia_vetor(n);

	for (i = 0; i < n; i++)
	{
		printf("%d ", v[i]);
	}
	printf("\n");

	#pragma omp parallel num_threads(4) shared(v) 
	{
		#pragma omp single 
		{
			merge_sort(0, n, v);
		}
	}

	for (i = 0; i < n; i++)
	{
		printf("%d ", v[i]);
	}
	printf("\n");
}