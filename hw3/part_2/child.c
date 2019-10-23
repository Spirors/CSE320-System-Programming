#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	
	sigset_t x;
	sigemptyset(&x);
	sigaddset(&x, SIGINT);
	sigprocmask(SIG_BLOCK, &x, NULL);

	int i;
	for(i = 1; i<=5; i++){
		printf("%d\n", i);
		sleep(1);
	}
	sigprocmask(SIG_UNBLOCK, &x, NULL);
	for(i = 6; i<=10; i++){
		printf("%d\n", i);
		sleep(1);
	}
	return 0;
}
