#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER 1024

int main(){
	char line[BUFFER];
	int argc;
	char* argv[30]; //number of arguments cap at 30
	char* path = "/bin/";
	char exepath[30]; //length of cmd cap at 30

	char *envp[] = {
		0
	};

	while(1){
		printf("shell> ");
		
		fgets(line, BUFFER, stdin);
		line[strcspn(line, "\n")] = '\0';

		//If cmd is exit
		if(strcmp(line, "exit") == 0){
			break;
		}

		char* token;
		token  = strtok(line, " ");
		int i = 0;
		while(token != NULL){
			argv[i] = token;
			token = strtok(NULL, " ");
			i++;
		}
		argv[i] = NULL; //Each cmd end with a null terminator
		argc = i;

		char cmd[30];
		strcpy(cmd, argv[0]);
		
		strcpy(exepath, path);
		strcat(exepath, argv[0]);
		exepath[strcspn(exepath, "\n")] = '\0';
		argv[0] = exepath;

		pid_t pid = fork();
		if(pid == 0){
			execve(argv[0], argv, envp);
			fprintf(stderr, "Failed to execute %s\n", cmd);
		}
		wait(NULL);
	}
	return 0;
}
