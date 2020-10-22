/* #define IMPRIME 1 */
/*multiplicacao de matrizes: outer-product*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
// #include <sys/time.h>
// #include "mede_time.h"

int SIZE;

double a[2000][2000];
double b[2000][2000];
double c[2000][2000]; 

/*inicia matriz*/
void init_matrix()
{   
  int i,j;
  #pragma omp parallel num_threads(2) shared(a,b,c) private(i,j)
  {
    #pragma omp for
    for(i = 0; i < SIZE; i++){  
      for (j = 0; j < SIZE; j++){
        a[i][j] = 1.0;
        b[i][j] = 1.0;
        c[i][j] = (double)0.0;
      }
    }
  }
}


/*multiplicacao*/
void mat_mul()
{
  int i,j,k;
  #pragma omp parallel num_threads(2) shared (a,b,c) private(i,j,k)
  {
    #pragma omp for    
    for(i = 0; i < SIZE; i++) {
      for (k = 0; k < SIZE; k++) 
        for (j = 0; j < SIZE; j++)
          c[i][j] = c[i][j] + a[i][k] * b[k][j];
    }
  }
}


/*imprime matriz */
void print_mat(a)
double a[2000][2000];
{
  int i,j;

  for (i = 0; i < SIZE; i++){
    for (j = 0; j < SIZE; j++)
      printf (" %10.2f", a[i][j]);
    puts("");
  }

  puts("");
}

/*imprime resultado*/
void print_mats(a, b, c)
double a[2000][2000],b[2000][2000],c[2000][2000];
{
  puts("Matriz A");
  print_mat(a);

  puts ("Matriz B");
  print_mat (b);

  puts ("Matriz C");
  print_mat (c);
}
 
void main(argc, argv)
int argc;
char **argv;
{

  if (argc == 2){ 
    SIZE = atoi(argv[1]);
    printf("Matriz %dx%d\n",SIZE,SIZE);
  }

  else{
    printf("Tamanho da matriz: ");
    scanf("%d",&SIZE);
  }
 
  init_matrix();
  // TIMER_CLEAR;
  // TIMER_START; 
  mat_mul();
  // TIMER_STOP;
  // printf ("TEMPO [%d]: %12.7f\n",SIZE,TIMER_ELAPSED );
  printf("c[3][3]=%f c[%d][%d]=%f\n",c[3][3],SIZE-1,SIZE-1,c[SIZE-1][SIZE-1]);
#ifdef IMPRIME 
  print_mats(a,b,c);
#endif 
}
