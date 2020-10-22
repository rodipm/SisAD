#include <stdio.h>
#include <omp.h>
#include "mede_time.h"
#include <sys/time.h>

int n;
int fib(int n)
{
  int i, j;
  if(n < 2)
    return n;
  
  else 
    if(n < 20){
      i = fib(n-1);
      j = fib(n-2);
      return i+j;
    }
    
    else{
      #pragma omp task shared(i) 
      {
        i = fib(n-1);
      }
      #pragma omp task shared(j) 
      {
        j = fib(n-2);
      }
    }

    #pragma omp taskwait
    return i+j;
}

void main()
{
  printf("n:");
  scanf("%d",&n);
  printf("MAIN n=%d\n",n);
 
  TIMER_CLEAR;
  TIMER_START; 
 
  #pragma omp parallel num_threads(2)
  { 
    #pragma omp single 
    printf("fib(%d)=%d\n",n,fib(n));
  }
 
  TIMER_STOP;
  printf ("TEMPO : %12.7f\n",TIMER_ELAPSED );
}

