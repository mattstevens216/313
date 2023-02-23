#include "semaphore.h"
#include <queue>
#include <string>

#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H

class BoundedBuffer{
	private:
	//none so far	
	public:
		//declaratiions
		Semaphore *sem, *full, *empty;
		std::queue<std::string> boundBuffer;
		//functions
		BoundedBuffer(int buf){
			sem = new Semaphore(1); //initialize mutex to be 1 (true)
			full = new Semaphore(0); //initialize full to be 0 (false)
			empty = new Semaphore(buf); //depends on buffer size.
		}
		~BoundedBuffer(){
			delete sem;
			delete full;
			delete empty; //any new must be deleted
		}
		void push(std::string str){
			empty->P(); //decrement empty to make it false
			sem->P(); //locks semaphore
			boundBuffer.push(str); //queue push not our function :P
			sem->V(); //unlocks semaphore (may need to check on this i cant remember if it is the opposite.
			full->V();//increment full
		}
		std::string pop(){
			full->P(); //decrement empty to make it false
			sem->P(); //locks semaphore
			std::string front = boundBuffer.front();
			boundBuffer.pop(); //queue pop not our function :P
			sem->V(); //unlocks semaphore (may need to check on this i cant remember if it is the opposite.
			empty->V();//increment empty
			return front;
		}
};
#endif
			
