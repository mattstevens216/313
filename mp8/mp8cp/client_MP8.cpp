/*
    File: client_MP8.cpp

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
    As in MP8 no additional includes are required
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

/*
    This next included file will need to be written from scratch, along with
    semaphore.h and (if you choose) their corresponding .cpp files.
 */

#include "boundedbuffer.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/
struct storageStruct{

    BoundedBuffer *request_buff;
    std::string str1;
    int number;
};

struct workerStruct{
    NetworkRequestChannel *chan;
    BoundedBuffer *request_buff;
    BoundedBuffer *joe_buffer;
    BoundedBuffer *jane_buffer;
    BoundedBuffer *john_buffer;
    pthread_mutex_t *p_lock;
};

struct statStruct {
    BoundedBuffer *stat_buffer;
    int number;
    std::vector<int> *stat_frequency;
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
    this in MP8, it hasn't changed since then so it
    won't be repeated.
 */
class atomic_standard_output {
    pthread_mutex_t console_lock;
public:
    atomic_standard_output() { pthread_mutex_init(&console_lock, NULL); }
    ~atomic_standard_output() { pthread_mutex_destroy(&console_lock); }
    void print(std::string s) {
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
//request threads put requests to Bounded Buffer
void* request_thread_function(void* arg) {
	storageStruct requester = *(storageStruct*)arg;
		std::string s;
		for(int i = 0; i < requester.number; i++){
			s = "data "+requester.str1;
			requester.request_buff->push(s);
		}
}

//Worker threads create channel to communicate with Server for the clients
void* worker_thread_function(void* arg) {
    workerStruct worker = *(workerStruct*)arg;
    while(true) {
        std::string request = worker.request_buff->pop();
		//std::cout << request << std::endl;
        std::string response = worker.chan->send_request(request);
		
		//std::cout << response << std::endl;
        //cerr<<request<<":"<<response<<endl;
        if(request == "data John Smith") {
            worker.john_buffer->push(response);
        }
        else if(request == "data Jane Smith") {
            worker.jane_buffer->push(response);
        }
        else if(request == "data Joe Smith") {
            worker.joe_buffer->push(response);
        }
        else if(request == "quit") {
            //cerr<<"Worker thread done."<<endl;
            delete worker.chan;
            break;
        }       
    }
}

//Stat threads compute the results
void* stat_thread_function(void* arg) {
    statStruct* stat = (statStruct*)arg;
    
    for(int i=0; i < stat->number; i++){
        std::string s = stat->stat_buffer->pop();
        stat->stat_frequency->at(stoi(s) /10) += 1;
    }
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
    int n = 10; //default number of requests per "patient"
    int b = 50; //default size of request_buffer
    int w = 10; //default number of worker threads
    std::string host_name = "127.0.0.1";
    int port_number;
    bool USE_ALTERNATE_FILE_OUTPUT = false;
    int opt = 0;
    while ((opt = getopt(argc, argv, "n:b:w:h:p:m:")) != -1) {
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
        case 'h':
			printf("optarg -h: %s\n", optarg); //this prints out 127.0.0.1
        	host_name = (optarg); //supposed to set the string...
			//printf("hostname in optarg: %s\n", host_name);
		//	std::cout << "hostname in optarg: " << host_name << std::endl;
		//	std::cout << "optarg after hostname: " << optarg << std::endl;
        	break;
        case 'p':
			port_number = atoi(optarg); //sets port to 11111.
			break;
		case 'm':
            USE_ALTERNATE_FILE_OUTPUT = true;
            break;
		default:
            std::cout << "This program can be invoked with the following flags:" << std::endl;
            std::cout << "-n [int]: number of requests per patient" << std::endl;
            std::cout << "-b [int]: maximum number of requests that will be allowed in the request buffer" << std::endl;
            std::cout << "-w [int]: number of worker threads" << std::endl;
            std::cout << "-h: print this message and set host to 127.0.0.1" << std::endl;
			std::cout << "-p: [int]: this sets the port number to the value given" << std::endl;
			std::cout << "-m: use output2.txt instead of output.txt for all file output" << std::endl; //purely for convenience, you may find it helpful since you have to make two graphs instead of one, and they require different data
            std::cout << "Example: ./client_solution -n 10000 -b 50 -w 120 -m" << std::endl;
            std::cout << "If a given flag is not used, a default value will be given" << std::endl;
            std::cout << "to its corresponding variable. If an illegal option is detected," << std::endl;
            std::cout << "behavior is the same as using the -h flag." << std::endl;
            exit(0);
        }
    }
    	//printf("host: %s\n", &host_name); //prints out null...
		//std::cout << "hostname after command line: " << host_name << std::endl;
		//printf("port: %d\n", port_number); //prints out the port 11111.

	struct timeval start_time;
    	struct timeval finish_time;
    	int64_t start_usecs;
	int64_t finish_usecs;
	pthread_mutex_t lock;
/* 		std::ofstream ofs;
		if(USE_ALTERNATE_FILE_OUTPUT) ofs.open("output2.txt", std::ios::out | std::ios::app);
		else ofs.open("output.txt", std::ios::out | std::ios::app);
		 */

        BoundedBuffer request_buffer(b);
        BoundedBuffer john_buff(b);
        BoundedBuffer jane_buff(b);
        BoundedBuffer joe_buff(b);
        
        pthread_mutex_init(&lock, NULL);
        fflush(NULL);
	pthread_t reqThreads[3];
        pthread_t workThreads[w];
	pthread_t statThreads[3];
        
        storageStruct john_thread;
        john_thread.number = n;
        john_thread.request_buff = &request_buffer;
        john_thread.str1 = "John Smith";
        
        storageStruct jane_thread;
        jane_thread.number = n;
        jane_thread.request_buff = &request_buffer;
        jane_thread.str1 = "Jane Smith";
        
        storageStruct joe_thread;
        joe_thread.number = n;
        joe_thread.request_buff = &request_buffer;
        joe_thread.str1 = "Joe Smith";
        
        storageStruct* john = &john_thread;
        storageStruct* jane = &jane_thread;
        storageStruct* joe = &joe_thread;
        
        pthread_create(&reqThreads[0], NULL, &request_thread_function, john);
        pthread_create(&reqThreads[1], NULL, &request_thread_function, jane);
        pthread_create(&reqThreads[2], NULL, &request_thread_function, joe);
        gettimeofday(&start_time, NULL);
        //create worker threads
        for(int i=0; i<w; i++){
            NetworkRequestChannel *workerChannel = new NetworkRequestChannel(host_name, port_number);
            workerStruct wparams;
            wparams.request_buff = &request_buffer;
            wparams.john_buffer = &john_buff;
            wparams.jane_buffer = &jane_buff;
            wparams.joe_buffer = &joe_buff;
            wparams.p_lock = &lock;
            wparams.chan = workerChannel;
            
            workerStruct* worker = &wparams;
            pthread_create(&workThreads[i], NULL, &worker_thread_function, worker);
        }
        
    
        //create stat threads
        std::vector<int> john_frequency_count(10, 0);
        std::vector<int> jane_frequency_count(10, 0);
        std::vector<int> joe_frequency_count(10, 0);
        
        statStruct john_thread_stats;
        statStruct jane_thread_stats;
        statStruct joe_thread_stats;

        john_thread_stats.number = n;
        john_thread_stats.stat_frequency = &john_frequency_count;
        john_thread_stats.stat_buffer = &john_buff;

        jane_thread_stats.number = n;
        jane_thread_stats.stat_frequency = &jane_frequency_count;
        jane_thread_stats.stat_buffer = &jane_buff;
        
        joe_thread_stats.number = n;
        joe_thread_stats.stat_frequency = &joe_frequency_count;
        joe_thread_stats.stat_buffer = &joe_buff;
    
        statStruct* ptr1 = &john_thread_stats;
        statStruct* ptr2 = &jane_thread_stats;
        statStruct* ptr3 = &joe_thread_stats;
    
        pthread_create(&statThreads[0], NULL, &stat_thread_function, ptr1);   
        pthread_create(&statThreads[1], NULL, &stat_thread_function, ptr2);   
        pthread_create(&statThreads[2], NULL, &stat_thread_function, ptr3);   
        
        
        //begin joining threads
        
        //request threads
        for(int i=0; i<3; ++i){
            pthread_join(reqThreads[i], NULL);
        }
        
        //insert quits for worker threads
        for(int i=0; i<w; ++i) {
            request_buffer.push("quit");
        }
        
        //join the remaining threads
        for(int i = 0; i < 3; ++i){
            pthread_join(statThreads[i], NULL);
        }
        
        std::cout << "All threads joined..." << std::endl;
        
        pthread_mutex_destroy(&lock);

        gettimeofday(&finish_time, NULL);
        
        //start_usecs = (start_time.tv_sec * 1e6) + start_time.tv_usec;
        //finish_usecs = (finish_time.tv_sec * 1e6) + finish_time.tv_usec;
        double total_time = (finish_time.tv_sec - start_time.tv_sec) + (finish_time.tv_usec - start_time.tv_usec)/1000000.0;
	std::cout << total_time << std::endl;
	std::cout << "Finished!" << std::endl;
        
        
        std::string histogram_table = make_histogram_table("John Smith",
                "Jane Smith", "Joe Smith", &john_frequency_count,
                &jane_frequency_count, &joe_frequency_count);

        std::cout << "Results for n == " << n << ", w == " << w << std::endl;

        //std::cout << "Time to completion: " << std::to_string((finish_usecs - start_usecs * .0000001)) << " secs" << std::endl;

        std::cout << histogram_table << std::endl;

        //ofs.close();
/*         std::cout << "Sleeping..." << std::endl;
        usleep(10000);
        std::string finale = workerStruct.chan->send_request("quit");
        std::cout << "Finale: " << finale << std::endl; */
  
    /*
    Do what you did in MP7, but remember we use NetworkReqChannel Here.
    */
}
