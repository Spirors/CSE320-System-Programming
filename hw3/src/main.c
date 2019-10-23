#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>

#include "defs.h"
#include "artist_ds.h"

#define BUFFER 255

struct artist_list* head = NULL;

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

void chld_handler(int n){
	int status;
	pid_t pid;
	while((pid = waitpid(-1, &status, WNOHANG)) > 0){
		if(pid_exist(head, pid) == 0){
			delete_artist(&head, pid);
		}
	}
}

void assign(pid_t pidX){
	kill(pidX, SIGUSR1);
	sleep(1);
}

void withdraw(pid_t pidX){
	kill(pidX, SIGUSR2);
	sleep(1);
}
void terminate(pid_t pidX){
	kill(pidX, SIGTERM);
	sleep(1);
}

int main(void){

	char line[BUFFER];
	
	int N;
	pid_t pid;
	char cmd[10];

	struct artist_list* artist;

	signal(SIGINT, int_handler);
	signal(SIGCHLD, chld_handler);

	while(1){
		cse320_print("shell> ");

		fgets(line, BUFFER, stdin);
		line[strcspn(line, "\n")] = '\0';
		lowercase(line);

		if(strcmp(line, "exit") == 0){
			struct artist_list* temp = head;
			struct artist_list* curr;
			while(temp != NULL){
				if(strcmp(temp->status, "ASSIGNED") == 0){
					withdraw(temp->pid);
				}
				pid_t temp_pid = temp->pid;
				curr = temp->next_artist;
				delete_artist(&head, temp->pid);
				terminate(temp_pid);
				temp = curr;
			}
			free_artist_list(head);
			head = NULL;
			temp = NULL;
			curr = NULL;
			break;
		}else if(strcmp(line, "help") == 0){
			cse320_print("help: give information about available commands\n");
			cse320_print("date: print today's date\n");
			cse320_print("hire [N]: hire N artist\n");
			cse320_print("fire [X]: fire artist X\n");
			cse320_print("fireall: fire all artist\n");
			cse320_print("assign [X]: assign artist X to a job\n");
			cse320_print("withdraw [X]: withdrwa artist X from job\n");
			cse320_print("list: print out all artist\n");
			cse320_print("exit: exit out of the program\n");
		}else if(strcmp(line, "date") == 0){
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
			struct artist_list* temp = head;
			struct artist_list* curr;
			while(temp != NULL){
				if(strcmp(temp->status, "ASSIGNED") == 0){
					withdraw(temp->pid);
				}
				pid_t temp_pid = temp->pid;
				curr = temp->next_artist;
				delete_artist(&head, temp->pid);
				terminate(temp_pid);
				temp = curr;
			}
			free_artist_list(head);
			head = NULL;
			temp = NULL;
			curr = NULL;
		}else if(strcmp(line, "list") == 0){
			print_list(head);
		}else if(count_word(line) == 2){
			if(strncmp(line, "hire", 4) == 0){
				if(sscanf(line, "%s %d", cmd, &N) == 2){
					int i;
					char* argv[] = {
						"./artist",
						0
					};
					for(i = 0; i < N; i++){
						
						pid = fork();
						
						if(pid == 0){
							execve(argv[0], argv, NULL);

							if(artist != NULL){
								if(artist->status != NULL){
									free(artist->status);
								}
								free(artist);
							}

						}else{
							artist = new_artist(pid);
							sortedInsert_artist(&head, artist);
						}
					}
				}
			}else if(strncmp(line, "fire", 4) == 0){
				if(sscanf(line, "%s %d", cmd, &pid) == 2){
					if(strcmp(getstatus(head, pid), "ASSIGNED") == 0){
						withdraw(pid);
					}
					delete_artist(&head, pid);
					terminate(pid);
				}
			}else if(strncmp(line, "assign", 6) == 0){
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
		}
	}
    return 0;
}
