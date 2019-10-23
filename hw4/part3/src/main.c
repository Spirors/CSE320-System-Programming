#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>

#include "defs.h"
#include "artist_ds.h"

#define BUFFER 255

//Global Variables
struct artist_list* head = NULL;
int readcnt = 0;
sem_t mutex, w;

void *thread(void *vargp);

void lowercase(char* str){
	for(int i = 0; str[i]; i++){
		str[i] = tolower(str[i]);
	}
}

int count_word(char* s){ 
	int count = 1; 
	int i; 
	for (i = 0; s[i] != '\0';i++){ 
		if (s[i] == ' '){ 
			count++; 
		} 
	} 
	return count; 
}

void int_handler(int n){
	sigset_t x;
	sigemptyset(&x);
	sigaddset(&x, SIGINT);
	sigprocmask(SIG_BLOCK, &x, NULL);
}

int main(void){

	char line[BUFFER];
	
	pthread_t tid;
	int N;
	char cmd[10];

	struct artist_list* artist;

	signal(SIGINT, int_handler);
	
	sem_init(&mutex, 0, 1);
	sem_init(&w, 0, 1);

	while(1){
		printf("shell> ");

		fgets(line, BUFFER, stdin);
		line[strcspn(line, "\n")] = '\0';
		lowercase(line);

		if(strcmp(line, "exit") == 0){
			free_artist_list(head);
			head = NULL;
			break;
		}else if(strcmp(line, "help") == 0){
			cse320_print("help: give information about available commands\n");
			cse320_print("date: print today's date\n");
			cse320_print("hire [N]: hire N artist\n");
			cse320_print("fire [X]: fire artist X\n");
			cse320_print("fireall: fire all artist\n");
			//cse320_print("assign [X]: assign artist X to a job\n");
			//cse320_print("withdraw [X]: withdrwa artist X from job\n");
			cse320_print("list: print out all artist\n");
			cse320_print("exit: exit out of the program\n");
		}else if(strcmp(line, "date") == 0){
			pid_t pid;
			char* argv[] = {
				"/bin/date",
				0
			};
			
			pid = fork();
			if(pid == 0){
				execve(argv[0], argv, NULL);
			}
			wait(NULL);
		}else if(strcmp(line, "fireall") == 0){
			free_artist_list(head);
			head = NULL;
		}else if(strcmp(line, "list") == 0){
			print_list(head);
		}else if(count_word(line) == 2){
			if(strncmp(line, "hire", 4) == 0){
				if(sscanf(line, "%s %d", cmd, &N) == 2){
					int i;
					for(i = 0; i < N; i++){
						pthread_create(&tid, NULL, thread, NULL);
						artist = new_artist(tid);
						sortedInsert_artist(&head, artist);
					}
				}
			}else if(strncmp(line, "fire", 4) == 0){
				if(sscanf(line, "%s %ld", cmd, &tid) == 2){
					delete_artist(&head, tid);
				}
			}	
		
		/*
			else if(strncmp(line, "assign", 6) == 0){
				if(sscanf(line, "%s %d", cmd, &pid) == 2){
					if(strcmp(getstatus(head, pid), "WAITING") == 0){
						assign(pid);
					}
					update_status(&head, pid, "ASSIGNED");
				}
			}else if(strncmp(line, "withdraw", 8) == 0){
				if(sscanf(line, "%s %d", cmd, &pid) == 2){
					if(strcmp(getstatus(head, pid), "ASSIGNED") == 0){
						withdraw(pid);
					}
					update_status(&head, pid, "WAITING");
				}
			}
		*/

		}
	}
    return 0;
}
void *thread(void *vargp){
	pthread_t tid = pthread_self();
	pthread_detach(tid);
	int exist = 1;
	while(exist){
		sem_wait(&mutex);
		readcnt++;
		if(readcnt == 1){
			sem_wait(&w);
		}
		sem_post(&mutex);

		if(tid_exist(head, tid) != 0){
			exist = 0;
		}

		sem_wait(&mutex);
		readcnt--;
		if(readcnt == 0){
			sem_post(&w);
		}
		sem_post(&mutex);
	}
	return NULL;
}
