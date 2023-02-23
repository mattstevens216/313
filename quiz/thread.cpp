#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <climits>
#include "thread.h"

/* Question 1 */
#define NTHREADS 10
thread_t threads[NTHREADS];

void go(int n){
    std::cout << "Hello from thread " << n << std::endl;
    thread_exit(100 + n);
}

int main() {
    for(int i = 0; i < NTHREADS; i++){
        thread_create(&threads[i], &go, i); 
    }  
    
    for(int i = 0; i < NTHREADS; i++){
        int exitValue = thread_join(threads[i]);
        std::cout << "Thread " << i << " returned with " << exitValue << std::endl;
    }

    std::cout << "Main thread done. " << std::endl;
    return 0;
}
