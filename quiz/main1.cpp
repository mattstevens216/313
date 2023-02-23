#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(){
	char buf[] = "abc";
	int r = open("foo.txt", O_RDWR);
	int r1 = 0;
	int r2 = open("foo.txt", O_RDWR);
	dup2(r, r1);
	read(r, buf, 1);
	read(r2, buf, 2);
	write(r, buf, 3);
	read(r2, buf, 1);
	write(r1, buf, 1);
	return 0;
}
