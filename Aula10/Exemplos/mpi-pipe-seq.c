/*
tarefa1:X = A x B
tarefa2:Y = E x F
tarefa3:Z = C x D
tarefa4: W = X + Y + Z + W i imprime 10 primeiros elementos da linha 499 de W 
*/

#include <stdio.h>
#include <sys/time.h>
#include "mede_time.h"

#define n 1000

int X[n][n],Y[n][n],Z[n][n],W[n][n];

void tarefa1() 
{   
    int i,j,k;
    for(j = 0; j < n; j++)
        X[0][j] = (n*2-j)%10;

    for(i = 1; i < n; i++){
        X[i][0] = (i*2)%10; 

        for(j = 1; j < n; j++) 
            for(k = 0 ; k < n; k++) 
                X[i][j] = X[i][j-1]*3 - (X[i-1][k]*(k+4))%30;
    }

    printf("tarefa1 FIM\n");
    fflush(stdout);
}

void tarefa2() 
{   
    int i,j,k;
    for(j = 0; j < n; j++) 
        Y[0][j] = (n*3-j)%15;
      
    for(i = 1; i < n; i++){
        for(j = 1; j < n; j++) 
            for(k = 0; k < n; k++) 
                Y[i][j] = Y[i][j-1]*4 - (Y[i-1][k]*(k+5))%50;
    }

    printf("tarefa2 FIM\n");
    fflush(stdout);
}

void tarefa3() 
{   
    int i,j,k;
    for(j = 0; j < n; j++)
        Z[0][j] = (n*2+j)%20;
    
    for(i = 1; i < n; i++){
        for(j = 1; j < n; j++) 
            for(k = 0; k < n; k++) 
                Z[i][j] = Z[i][j-1]*2 - (Z[i-1][k]*(k+3))%40;
    }

    printf("tarefa3 FIM\n");
    fflush(stdout);
}

void tarefa4() 
{ 
    int i,j,k;
  
    for(i = 0; i < n; i++) 
        for(j = 0 ; j < n; j++) 
            W[i][j] = X[i][j] + Y[i][j] + Z[i][j];
  
    printf("tarefa4 W[0][0]=%d W[300][400]=%d  W[999][999]=%d\n",W[0][0], W[300][400],W[999][999]);
    fflush(stdout);
    printf("tarefa4 FIM\n");
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    TIMER_CLEAR;
    TIMER_START;
    tarefa1();
    tarefa2();
    tarefa3();
    tarefa4();
    TIMER_STOP;
    printf("TEMPO=%f\n",TIMER_ELAPSED);

    return 0;
}