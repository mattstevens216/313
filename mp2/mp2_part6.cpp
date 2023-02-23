/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part6.cpp                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Standard Library Includes                                                  */
/* -------------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    int fds[2];
    char buf[1024];
    pipe(fds);
    if(fork() == 0){
    	close(fds[0]);
	dup2(fds[1], 1);
	execl("/bin/ls", "ls", "-la", (char*)0); 
    }
    else{
	close(fds[1]);
	wait(NULL);
	read(fds[0], buf, sizeof(buf));
	write(1, buf, sizeof(buf));
    }
    exit(0);
}
