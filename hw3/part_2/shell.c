#include <sys/wait.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <signal.h>

#define BUFFER 1024 

void handler(int n){
	sigset_t x;
	sigemptyset(&x);
	sigaddset(&x, SIGINT);
	sigprocmask(SIG_BLOCK, &x, NULL);
}

int main(){ 
	char line[BUFFER];
	char* argv[] = {
		"./child",
		0
	};

	signal(SIGINT, handler);
	
	while(1){ 
		printf("shell> "); 

		fgets(line, BUFFER, stdin); 
		line[strcspn(line, "\n")] = '\0'; 

		if(strcmp(line, "exit") == 0){ 
			break; 
		} 
		if(strcmp(line, "run") == 0){
			pid_t pid = fork(); 
			if(pid == 0){ 
				execve(argv[0], argv, NULL); 
			}
		}
		wait(NULL);
	} 
	return 0;
}
