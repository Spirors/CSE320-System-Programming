#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "defs.h"

int fibnum;
void *thread(void *vargp);

int fib(int n){ 
	if(n <= 1){
		return n;
	}
	return fib(n-1) + fib(n-2);
}

int main(int argc, char* argv[]){
	
	int N;

	if(argc != 2){
		return -1;
	}

	N = atoi(argv[1]);

	if(N < 0){
		return -1;
	}

	pthread_t tid;
	pthread_create(&tid, NULL, thread,(void*)&N);
	pthread_join(tid, NULL);
	cse320_printf("%d\n", fibnum);
}

void *thread(void *vargp){
	fibnum = fib(*((int*)vargp));
	pthread_exit(0);
}
