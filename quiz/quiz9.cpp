#include <signal.h>
#include <stdio.h>
//#include <thread>
//#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t pid;
void h1(int sig) {
  printf("eagle"); kill(pid, SIGUSR2);
}
void h2(int sig) {
  printf("sparrow");
  exit(0);
}
int main() {
  signal(SIGUSR2, h1);
  if ((pid = fork()) == 0) {
    signal(SIGUSR2, h2);
    kill(getppid(), SIGUSR2);
    while(1) {
    }
  }
  else {
    pid_t p; int status;
    //p = wait(&status);
    if ((p = wait(&status)) > 0) {
      printf("pigeon");
    }
  }
}
