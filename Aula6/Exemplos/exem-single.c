#include <stdio.h>
#include <omp.h>

int main()
{
  int a;
  omp_set_num_threads(4);
  #pragma omp parallel 
    {
       printf("TESTE %d\n",omp_get_thread_num());
       fflush(stdout);
       #pragma omp single
         {
            a=25;
            printf("SINGLE %d  a=%d\n",omp_get_thread_num(),a);
            // omp_get_thread_num retorna id da thread corrente
            fflush(stdout);
         }
       a++;
       printf("thread  %d  a=%d\n",omp_get_thread_num(),a);
       fflush(stdout);
  }
}

