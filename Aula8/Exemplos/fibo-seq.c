#include <stdio.h>
int i;
int n;

int fib(int n)
{
	int i, j;
  
	if(n < 2)
		return n;
  
	else {
		i = fib(n-1);
		j = fib(n-2);
		return i+j;
	}
}

int main(){
	printf("n:");
	scanf("%d",&n);
	printf("n=%d",n); 
	printf("fib(%d)=%d\n",n,fib(n));
}