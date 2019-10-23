#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "defs.h"

struct qsortData {
	int* arr;
	int start;
	int end;
};

struct qsortData* newData(int n){
	struct qsortData* data = (struct qsortData*)malloc(sizeof(struct qsortData));
	data->arr = (int*)malloc((n+1)*sizeof(int));
	data->start = 0;
	data->end = (n-1);
	
	return data;
}

void* quicksort(void* arg);
void swap(int* a, int* b);
int partition(int arr[], int low, int high);

//Global
struct qsortData* data = NULL;

int main(int argc, char* argv[]){

	if(argc != 2){
		return -1;
	}

	FILE *fp;
	fp = fopen(argv[1], "r");

	if(fp == NULL){
		return -1;
	}
	
	int n, i;
	fscanf(fp, "%d", &n);
	
	if(n <= 0){
		fclose(fp);
		return -1;
	}

	data = newData(n);

	for(i = 0; i < n; i++){
		fscanf(fp, "%d", &(data->arr)[i]);
	}
	fclose(fp);
	
	quicksort(data);

	for(i = 0; i < n; i++){
		cse320_printf("%d ", (data->arr)[i]);
	}
	free(data->arr);
	free(data);
	data = NULL;
	return 0;
}

void swap(int* a, int* b){
	int t = *a;
	*a = *b;
	*b = t;
}

int partition(int arr[], int low, int high){
	int pivot = arr[high];
	int i = (low-1);
	int j;
	for(j = low; j <= high-1; j++){
		if(arr[j] <= pivot){
			i++;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i+1], &arr[high]);
	return (i+1);
}

void* quicksort(void* arg){
	struct qsortData* data = arg;
	struct qsortData other;
	int low, high, pi;
	low = data->start;
	high = data->end;

	pthread_t tid;

	if(low < high){
		pi = partition(data->arr, low, high);

		other.start = pi+1;
		other.end = high;
		other.arr = data->arr;

		if(pthread_create(&tid, NULL, quicksort, &other)){
			return NULL;
		}

		data->start = low;
		data->end = pi-1;
		if(data->end < 0){
			data->end = 0;
		}
		quicksort(data);

		pthread_join(tid, NULL);
	}

}
