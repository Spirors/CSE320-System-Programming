#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#include "defs.h"

#define BUFFER 255

struct museum_ds {
	int id;
	int visitor;
	int status; //0 for open, 1 for closed
};

void lowercase(char* str){ 
	for(int i = 0; str[i]; i++){ 
		str[i] = tolower(str[i]); 
	} 
} 

void int_handler(int n){
	sigset_t x;
	sigemptyset(&x);
	sigaddset(&x, SIGINT);
	sigprocmask(SIG_BLOCK, &x, NULL);
}

struct museum_ds* newMuseum(int n){
	struct museum_ds* new_museum = (struct museum_ds*)malloc(sizeof(struct museum_ds));

	new_museum->id = n;
	new_museum->visitor = 1;
	new_museum->status = 0;

	return new_museum;
}

//Function that modifies Global Variable
void visitor_in(int N);
void visitor_out(int N);
void museum_info();
void museum_close(int N);
void museum_clean();

void writer(int N, int mode);
void reader();

//Thread functions
void* start_thread(void* arg);
void* writer_in(void* arg);
void* writer_out(void* arg);

//Global Variables
struct museum_ds* museums[5];
int readcnt = 0;
sem_t mutex, w;

int main(){

	int start_flag = 0;

	char line[BUFFER];
	
	pthread_t tid;
	int N, X;
	char cmd[5];

	signal(SIGINT, int_handler);
	
	int i;
	for(i = 0; i < 5; i++){
		museums[i] = newMuseum(i+1);
	}

	sem_init(&mutex, 0, 1);
	sem_init(&w, 0, 1);

	while(1){
		printf("shell> ");
		
		fgets(line, BUFFER, stdin);
		line[strcspn(line, "\n")] = '\0';
		lowercase(line);

		if(strcmp(line, "exit") == 0){
			museum_clean();
			break;
		}else if(strcmp(line, "start") == 0){
			if(start_flag == 0){
				pthread_create(&tid, NULL, start_thread, NULL);
				start_flag++;
			}else{
				fprintf(stderr, "you have already called start\n");
			}
		}else if(strcmp(line, "info") == 0){
			museum_info();
		}else if(strncmp(line, "in", 2) == 0){
			if(sscanf(line, "%s %d %d", cmd, &N, &X) == 3){
				pthread_t tids[X];
				int r;
				for(i = 0; i < X; i++){
					r = pthread_create(&tid, NULL, writer_in, (void*)&N);
					if(r != 0){
						perror("thread not created\n");
					}
					if(r == 0){
						tids[i] = tid;
					}
				}
				for(i = 0; i < X; i++){
					pthread_join(tids[i], NULL);
				}
			}
		}else if(strncmp(line, "out", 3) == 0){
			if(sscanf(line, "%s %d %d", cmd, &N, &X) == 3){
				pthread_t tids[X];
				int r;
				for(i = 0; i < X; i++){
					r = pthread_create(&tid, NULL, writer_out, (void*)&N);
					if(r != 0){
						perror("thread not created\n");
					}
					if(r == 0){
						tids[i] = tid;
					}
				}
				for(i = 0; i < X; i++){
					pthread_join(tids[i], NULL);
				}
			}
		}
	}
	return 0;
}

void visitor_in(int N){
	if(N >= 1 && N <= 5){
		museums[N-1]->visitor++;
	}else{
		fprintf(stderr, "N is not between 1-5\n");
	}
}

void visitor_out(int N){
	if(N >= 1 && N <= 5){
		if(museums[N-1]->visitor > 0){
			museums[N-1]->visitor--;
		}
	}else{
		fprintf(stderr, "N is not between 1-5\n");
	}
}

void museum_info(){
	int i;
	const char* open = "OPEN";
	const char* closed = "CLOSED";
	for(i = 0; i < 5; i++){
		if(museums[i]->status == 0){
			cse320_printf("%d:%d:%s\n", museums[i]->id, museums[i]->visitor, open);
		}else if(museums[i]->status == 1){
			cse320_printf("%d:%d:%s\n", museums[i]->id, museums[i]->visitor, closed);
		}
	}
}

void museum_close(int N){
	museums[N-1]->status = 1;
}

void museum_clean(){
	int i;
	for(i = 0; i < 5; i++){
		free(museums[i]);
		museums[i] = NULL;
	}
}

void reader(){
	int i;
	int close = -1;

	while(1){ 
		sem_wait(&mutex); 
		readcnt++; 
		if(readcnt == 1){ 
			sem_wait(&w); 
		} 
		sem_post(&mutex); 

		for(i = 0; i < 5; i++){
			if(museums[i]->visitor == 0){
				close = i+1;
				break;
			}
		}
		
		sem_wait(&mutex); 
		readcnt--; 
		if(readcnt == 0){ 
			sem_post(&w);
		} 
		sem_post(&mutex);
		break;
	}
	if(close != -1){
		writer(close, 2);
	}
}

void* start_thread(void* arg){
	while(1){
		reader();
		sleep(3);
	}
}

void writer(int N, int mode){
	while(1){
		sem_wait(&w);

		if(mode == 0){
			visitor_in(N);
		}else if(mode == 1){
			visitor_out(N);
		}else if(mode == 2){
			museum_close(N);
		}
		
		sem_post(&w);
		break;
	}
}
void* writer_in(void* arg){
	int N = *((int*)arg);
	writer(N, 0);
	return NULL;
}

void* writer_out(void* arg){
	int N = *((int*)arg);
	writer(N, 1);
	return NULL;
}
