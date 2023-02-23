/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part7.cpp                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Standard Library Includes                                                  */
/* -------------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main()
{
    int fds[2];
    int nbytes;
    char buf[1024];
    if(pipe(fds) == -1){
	fprintf(stderr, "parent failed to pipe\n");
	return -1;
    };

    pid_t mypid;
    if((mypid = fork()) == -1){
	fprintf(stderr, "parent fork failed\n");
	return -1;
    } else if(mypid == 0){
       	int ls_to_tr[2];
    	if(pipe(ls_to_tr) == -1){
	   fprintf(stderr, "parent failed to pipe)");
	   return -1;
	};
	if((mypid = fork()) == -1){
	   fprintf(stderr, "Child fork() failed\n");
	   return -1;
	} else if (mypid == 0){
		close(ls_to_tr[READ]);
		close(STDOUT_FILENO);
		dup2(ls_to_tr[WRITE], STDOUT_FILENO);
		execl("/bin/ls", "ls", "-la", (char*)0);
		fprintf(stderr, "child of child failed. \n");
		return -1;
    	} else {	
	        close(ls_to_tr[WRITE]);
		close(STDIN_FILENO);
		dup2(ls_to_tr[READ], STDIN_FILENO);
		close(STDOUT_FILENO);
		dup2(fds[WRITE], STDOUT_FILENO);
		execl("/bin/tr", "tr", "[a-zA-Z0-9]", "a", (char*)0);
		fprintf(stderr, "child of child failed. \n");
    		return -1;
	}
    } else {
	waitpid(mypid, NULL, 0);
	close(fds[WRITE]);
	while((nbytes = read(fds[READ], buf, sizeof(buf))) > 0){
	   write(STDOUT_FILENO, buf, nbytes);
        }
    }
    exit(0);
}

