#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h> 


int main(int ac, char *av[]){
	void inthandler(int);
	void quithandler(int);
	int INPUTLEN = 100;
	char input[INPUTLEN];
	int nchars;
	signal(SIGINT, inthandler);
	signal(SIGQUIT, quithandler);
	do{
		printf("\nType a message\n");
		nchars = read(0, input, (INPUTLEN - 1));
		if(nchars == -1)
			perror("read returned an error");
		else{
			input[nchars] = '\0';
			printf("You typed: %s", input);
		}
	} while(strncmp(input, "quit", 4) != 0);
}
void inthandler(int s){
	printf("Received Signal %d .... waiting\n", s);
	sleep(2);
	printf("Leaving inthandler \n");
}
void quithandler(int s){
	printf("Received Signal %d .... waiting\n", s);
	sleep(3);
	printf("Leaving quithandler \n");
}
