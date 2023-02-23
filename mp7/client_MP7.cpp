/*
    File: client_E8.cpp
    Author: J. Higginbotham
    Department of Computer Science
    Texas A&M University
    Date  : 2016/05/21
    Based on original code by: Dr. R. Bettati, PhD
    Department of Computer Science
    Texas A&M University
    Date  : 2013/01/31
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */
    /* -- This might be a good place to put the size of
        of the patient response buffers -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*
    As in MP7 no additional includes are required
    to complete the assignment, but you're welcome to use
    any that you think would help.
*/
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <sstream>
#include <sys/time.h>
#include <assert.h>
#include <fstream>
#include <numeric>
#include <vector>
#include "reqchannel.h"
#include "boundedbuffer.h"


/*
    This next included file will need to be written from scratch, along with
    semaphore.h and (if you choose) their corresponding .cpp files.
 */

//#include "bounded_buffer.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

struct storageStruct{
    BoundedBuffer *request;
    std::string str1;
    int number;
    storageStruct(BoundedBuffer* request, std::string str, int num) : request(request), str1(str), number(num){};
};

struct workerStruct{
    RequestChannel *chan;
    BoundedBuffer* request_buffer;
    std::vector<int>*johnFrequency;
    std::vector<int>*janeFrequency;
    std::vector<int>*joeFrequency;
    pthread_mutex_t *johnmtx;
    pthread_mutex_t *joemtx;
    pthread_mutex_t *janemtx;

    workerStruct(RequestChannel *chan, BoundedBuffer *request_buffer, std::vector<int> *johnFrequency,  std::vector<int> *janeFrequency,  std::vector<int> *joeFrequency,  pthread_mutex_t *johnmtx, pthread_mutex_t *joemtx,  pthread_mutex_t *janemtx){
        this->chan = chan;
        this->request_buffer = request_buffer;
        this->johnFrequency = johnFrequency;
        this->janeFrequency = janeFrequency;
        this->joeFrequency = joeFrequency;
        this->johnmtx = johnmtx;
        this->joemtx = joemtx;
        this->janemtx = janemtx;
    }
};

struct statStruct {
    int number;
	std::string statstr;
    std::vector<int> *freq;
    BoundedBuffer* stat_request_buffer;

};

/*
    All *_params structs are optional,
    but they might help since you still
	can't use global variables.
 */

/*
    This class can be used to write to standard output
    in a multithreaded environment. It's primary purpose
    is printing debug messages while multiple threads
    are in execution. You probably saw the explanation of
	this in MP6, it hasn't changed since then so it
	won't be repeated.
 */
class atomic_standard_output {
    pthread_mutex_t console_lock;
public:
    atomic_standard_output() { pthread_mutex_init(&console_lock, NULL); }
    ~atomic_standard_output() { pthread_mutex_destroy(&console_lock); }
    void print(std::string s){
        pthread_mutex_lock(&console_lock);
        std::cout << s << std::endl;
        pthread_mutex_unlock(&console_lock);
    }
};

atomic_standard_output threadsafe_standard_output;

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* HELPER FUNCTIONS */
/*--------------------------------------------------------------------------*/

std::string make_histogram_table(std::string name1, std::string name2,
        std::string name3, std::vector<int> *data1, std::vector<int> *data2,
        std::vector<int> *data3) {
	std::stringstream tablebuilder;
	tablebuilder << std::setw(25) << std::right << name1;
	tablebuilder << std::setw(15) << std::right << name2;
	tablebuilder << std::setw(15) << std::right << name3 << std::endl;
	for (int i = 0; i < data1->size(); ++i) {
		tablebuilder << std::setw(10) << std::left
		        << std::string(
		                std::to_string(i * 10) + "-"
		                        + std::to_string((i * 10) + 9));
		tablebuilder << std::setw(15) << std::right
		        << std::to_string(data1->at(i));
		tablebuilder << std::setw(15) << std::right
		        << std::to_string(data2->at(i));
		tablebuilder << std::setw(15) << std::right
		        << std::to_string(data3->at(i)) << std::endl;
	}
	tablebuilder << std::setw(10) << std::left << "Total";
	tablebuilder << std::setw(15) << std::right
	        << accumulate(data1->begin(), data1->end(), 0);
	tablebuilder << std::setw(15) << std::right
	        << accumulate(data2->begin(), data2->end(), 0);
	tablebuilder << std::setw(15) << std::right
	        << accumulate(data3->begin(), data3->end(), 0) << std::endl;

	return tablebuilder.str();
}

/*
    You'll need to fill these in.
*/
void* request_thread_function(void* arg) {
    storageStruct * requester = (storageStruct*) arg;
    for(int i = 0; i < requester->number; i++) { //3 request threads
        requester->request->push(requester->str1);
    }

}

void* worker_thread_function(void* arg) {
    workerStruct * worker = (workerStruct*) arg;
    std::string s = worker->chan->send_request("newthread"); //creates worker threads 
    RequestChannel *workerChannel = new RequestChannel(s, RequestChannel::CLIENT_SIDE);
    while(true){
        std::string request  = worker->request_buffer.front(); //not sure if fixed but we need to obtain the string before popping it off the queue.
            worker->request_buffer->pop();
			std::string response = workerChannel->send_request(request);

            if(request == "data John Smith") {
                worker->johnFrequency->at(stoi(response) / 10) += 1;
            }
            else if(request == "data Jane Smith") {
                worker->janeFrequency->at(stoi(response) / 10) += 1;
            }
            else if(request == "data Joe Smith") {
                worker->joeFrequency->at(stoi(response) / 10) += 1;
            }
            else if(request == "quit") {
                delete workerChannel;
        break;
        }       
    }
    pthread_exit(NULL);
    return NULL;

}

void* stat_thread_function(void* arg) {

    statStruct* s = (statStruct*)arg;
        for(int i = 0; i < s->number; i++) { //3 request threads
			s->stat_request_buffer->push(s->statstr);
    }
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
    int n = 10; //default number of requests per "patient"
    int b = 50; //default size of request_buffer
    int w = 10; //default number of worker threads
    bool USE_ALTERNATE_FILE_OUTPUT = false;
    int opt = 0;
    while ((opt = getopt(argc, argv, "n:b:w:mh")) != -1) {
        switch (opt) {
            case 'n':
                n = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 'w':
                w = atoi(optarg);
                break;
            case 'm':
                USE_ALTERNATE_FILE_OUTPUT = true;
                break;
            case 'h':
            default:
                std::cout << "This program can be invoked with the following flags:" << std::endl;
                std::cout << "-n [int]: number of requests per patient" << std::endl;
                std::cout << "-b [int]: maximum number of requests that will be allowed in the request buffer" << std::endl;
                std::cout << "-w [int]: number of worker threads" << std::endl;
                std::cout << "-m: use output2.txt instead of output.txt for all file output" << std::endl; //purely for convenience, you may find it helpful since you have to make two graphs instead of one, and they require different data
                std::cout << "-h: print this message and quit" << std::endl;
                std::cout << "Example: ./client_solution -n 10000 -b 50 -w 120 -m" << std::endl;
                std::cout << "If a given flag is not used, a default value will be given" << std::endl;
                std::cout << "to its corresponding variable. If an illegal option is detected," << std::endl;
                std::cout << "behavior is the same as using the -h flag." << std::endl;
                exit(0);
        }
    }

    int pid = fork();
	if (pid > 0) {
        struct timeval start_time;
        struct timeval finish_time;
        int64_t start_usecs;
        int64_t finish_usecs;
        std::ofstream ofs;
        if(USE_ALTERNATE_FILE_OUTPUT) ofs.open("output2.txt", std::ios::out | std::ios::app);
        else ofs.open("output.txt", std::ios::out | std::ios::app);

        std::cout << "n == " << n << std::endl;
        std::cout << "b == " << b << std::endl;
        std::cout << "w == " << w << std::endl;

        std::cout << "CLIENT STARTED:" << std::endl;
        std::cout << "Establishing control channel... " << std::flush;
        gettimeofday(&start_time, NULL);
	RequestChannel *chan = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
        gettimeofday(&start_time, NULL);
	double total_time = (finish_time.tv_sec - start_time.tv_sec)+(finish_time.tv_usec - start_time.tv_usec)/1000000.0;
	std::cout << total_time << std::endl;
	std::cout << "done." << std::endl;
	
        /*
            This time you're up a creek.
            What goes in this section of the code?
            
			The nature of this assignment
			prohibits the writing of helpful skeleton
			code, or rather, skeleton code that's as
			helpful as in MP6. However, much of
			what you learned and used in MP6
			can be reused here.
         */

        ofs.close();
	
        std::cout << "Sleeping..." << std::endl;
        usleep(10000);
        std::string finale = chan->send_request("quit");
        std::cout << "Finale: " << finale << std::endl;
    }
	else if (pid == 0)
		execl("dataserver", NULL);
}
