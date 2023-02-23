#include <pthread.h>
#include <iostream>
#include <stdio.h>

class Semaphore{
   private:

   public:
	//public variables
	pthread_mutex_t mutex;
	pthread_cond_t condition;
	int counter;
	//functions
	Semaphore(int x){
		pthread_mutex_init(&mutex, NULL);
		pthread_cond_init(&condition, NULL);
		counter = x;
	}


	~Semaphore(){
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&condition);
	}

	void V(){
		pthread_mutex_lock(&mutex);
		counter++;
		if(counter <= 0){
			pthread_cond_signal(&condition);
		}
		pthread_mutex_unlock(&mutex);
	}

	void P(){
		pthread_mutex_lock(&mutex);
		counter--;
		if(counter < 0){
			pthread_cond_wait(&condition, &mutex);
		}
		pthread_mutex_unlock(&mutex);
	}
};

