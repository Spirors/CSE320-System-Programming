#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "defs.h"

void h1(int n){
	pid_t pid = getpid();
	cse320_printf("ARTIST %d IS ASSIGNED TO A JOB\n", pid);
}

void h2(int n){
	pid_t pid = getpid();
	cse320_printf("ARTIST %d IS WITHDRAWN FROM A JOB\n", pid);
}

void main(){
	signal(SIGUSR1, h1);
	signal(SIGUSR2, h2);
	while(1);
}
