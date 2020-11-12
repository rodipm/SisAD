/*
tarefa1: Calcula X
tarefa2: Calcula Y
tarefa3: Inicializa T e calcula W (W = X * T + Y)
*/

#include <stdio.h>
#include <sys/time.h>
#include "mede_time.h"
#define n 1000

int X[n][n],Y[n][n],W[n][n];
int T[n][n];

void tarefa1() 
{   
	int i,j,k;
    
	for(j = 0; j < n; j++)
		X[0][j] = (n*2-j)%10;
    
	for(i = 1; i < n; i++){
		X[i][0] = (i*2)%10; 
         
		for(j = 1; j < n; j++) 
			for(k = 0; k < n; k++) 
				X[i][j] = (X[i][j-1]*3 - (X[i-1][k]*(k+4))%30)%100;
	}

	printf("X[0][0]=%d X[300][400]=%d  X[%d][%d]=%d\n",X[0][0], X[300][400],n-1,n-1,X[n-1][n-1]);
	fflush(stdout);
	printf("tarefa1 FIM\n");
	fflush(stdout);
}

void tarefa2() 
{   
    int i,j,k;
    for(j = 0; j < n; j++)
		Y[0][j] = (n*3-j)%15;

    for(i=1;i<n;i++){
		Y[i][0]=(i*3)%15;
         
		for(j = 1; j < n; j++) 
			for(k = 0; k < n; k++) 
				Y[i][j] = (Y[i][j-1]*4-(Y[i-1][k]*(k+5))%50)%100;
    }

	printf("Y[0][0]=%d Y[300][400]=%d  Y[%d][%d]=%d\n",Y[0][0], Y[300][400],n-1,n-1,Y[n-1][n-1]);
	fflush(stdout);
	printf("tarefa2 FIM\n");
    fflush(stdout);
}

void tarefa3() 
{ 
	int i,j,k;
	for(i = 0; i < n; i++) 
		for(j = 0; j < n; j++) 
			T[i][j] = (n*3-i+j)%15;
    
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++) 
			for(k = 0; k < n; k++) 
				W[i][j] = W[i][j] + X[i][k]*T[k][j];
    
	for(i = 0; i < n; i++)
          for(j = 0; j < n; j++) 
             W[i][j] = W[i][j] + Y[i][j];

	printf("tarefa 3 W[0][0]=%d W[300][400]=%d  W[999][999]=%d\n",W[0][0], W[300][400],W[999][999]);
	fflush(stdout);
    printf("tarefa3 FIM\n");
    fflush(stdout);
}

int main(int argc, char *argv[])
{
	TIMER_CLEAR;
	TIMER_START;
	tarefa1();
	tarefa2();
	tarefa3();
	TIMER_STOP;
	printf("TEMPO=%f\n",TIMER_ELAPSED);

	return 0;
}