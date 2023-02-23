//
//  SafeBuffer.cpp
//
//
//  Created by Joshua Higginbotham on 11/4/15.
//
//

#include "SafeBuffer.h"
#include <string>
#include <queue>

SafeBuffer::SafeBuffer() {

}

SafeBuffer::~SafeBuffer() {

}

int SafeBuffer::size() { //return the size of fifo
    return fifo.size();
}

void SafeBuffer::push_back(std::string str) { //push str to the back of the queue
	mutex.lock();
	fifo.push(str);
	mutex.unlock();
}

std::string SafeBuffer::retrieve_front() { //grab the front element of the queue.
	mutex.lock();
	std::string front = fifo.front();
	fifo.pop();
	mutex.unlock();
	return front;
}
