#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(){
	char buf[] = "ab";
	int r = open("foo.txt", O_RDONLY);
	int r1, r2, pid;
	r1 = dup(r);
	read(r, buf, 1);
	if((pid=fork())==0){
		r1 = open("foo.txt", O_RDONLY);
	}
	else{
		waitpid(pid, NULL, 0);
	}
	read(r1, buf, 1);
	printf("%s", buf);
	return 0;
}
