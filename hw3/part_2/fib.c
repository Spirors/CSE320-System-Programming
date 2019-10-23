#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>

int fib(uint64_t f[], int n){
	int i;
	f[0] = 0;
	f[1] = 1;
	for(i=2; i<=n; i++){
		f[i] = f[i-1] + f[i-2];
	}
	return f[n];
}
int fib_lastDigit(uint64_t n){
	uint64_t f[60] = {0};
	fib(f, 60);
	return f[n%60]%10;
}
void handler(int n){
	int status;
	wait(&status);
	if(WIFEXITED(status)){ 
		int exit_status = WEXITSTATUS(status);         
		printf("%d\n", exit_status); 
	}
}

int main(int argc, char *argv[]){
	if(argc<=1){
		printf("No argument");
		exit(1);
	}
	uint64_t N;
	sscanf(argv[1],"%"SCNu64, &N);
	
	signal(SIGCHLD, handler);

	pid_t pid;
	
	pid = fork();
	if(pid == 0){
		int fibnum;
		int lastdigit;
		fibnum = fib_lastDigit(N);
		lastdigit = fibnum % 10;
		_exit(lastdigit);
	}
	pause();
	
	return 0;
}
