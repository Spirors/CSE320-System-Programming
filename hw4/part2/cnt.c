#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#include "defs.h"

struct number_counter {
	int zero_counter;
	int one_counter;
	int two_counter;
};

struct number_counter counters;
void *thread(void *vargp);
sem_t mutex;

int main(){
	counters.zero_counter = 0;
	counters.one_counter = 0;
	counters.two_counter = 0;

	char* file1 = "file1.dat";
	char* file2 = "file2.dat";
	char* file3 = "file3.dat";
	
	sem_init(&mutex, 0, 1); 
	pthread_t tid1, tid2, tid3;
	pthread_create(&tid1, NULL, thread, (void*)file1);
	pthread_create(&tid2, NULL, thread, (void*)file2);
	pthread_create(&tid3, NULL, thread, (void*)file3);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);

	cse320_printf("%d %d %d\n", counters.zero_counter, 
								counters.one_counter, 
								counters.two_counter);
}

void *thread(void *vargp){
	char *filename = (char*)vargp;
	FILE *fp;
	fp = fopen(filename, "r");
	
	if(fp == NULL){
		pthread_exit(NULL);
	}
	
	int N, i;
	fscanf(fp, "%d", &N);
	if(N <= 0){
		fclose(fp);
		pthread_exit(NULL);
	}
	int arr[N];
	for(i = 0; i < N; i++){
		fscanf(fp, "%d", &arr[i]);
		
		sem_wait(&mutex);
		if(arr[i] == 0){
			counters.zero_counter++;
		}
		if(arr[i] == 1){
			counters.one_counter++;
		}
		if(arr[i] == 2){
			counters.two_counter++;
		}
		sem_post(&mutex);
	}
	fclose(fp);
	return NULL;
}
