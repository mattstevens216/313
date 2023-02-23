//Jason Alonzo & Matthew Stevens

#include "RoundRobin.h"

//No need to change it
RoundRobin::RoundRobin() {
}


/*
This is a constructor for RoundRobin Scheduler, you should use the extractProcessInfo function first
to load process information to process_info and then sort process by arrival time;
Also, fill in the procesVec with shared_ptr
Also initialize time_quantum
*/
RoundRobin::RoundRobin(string file, int tq) {
	extractProcessInfo(file);

	std::sort(begin(process_info), end(process_info), [](ProcessInfo const &t1, ProcessInfo const &t2) {
		return get<1>(t1) < get<1>(t2);
	}); //sorting the processes by arrival time
	
	for(auto it : process_info)
	{
		Process p(get<0>(it), get<1>(it), get<2>(it));
		auto proc = make_shared<Process>(p);
		processVec.push_back(proc);
	}

	time_quantum = tq;
	
}

void RoundRobin::set_time_quantum(int quantum) {
	time_quantum = quantum;
}

int RoundRobin::get_time_quantum() {
	return time_quantum;
}


//Schedule tasks based on RoundRobin Rule
//the jobs are put in the order the arrived
//Make sure you print out the information like we put in the document
void RoundRobin::schedule_tasks() {
	/*
	Do this part.
	*/

	float wait_time = 0;
	float response_time = 0;	
	int time = 2; 
	int i = 0;

	while(!(processVec.empty())){

		if(i > processVec.size()-1){
			 	i = 0;
			}

			auto p = processVec[i];
			while(p->get_arrival_time() > time){
				auto p = processVec[i++];
			}

			for(int i = 0; i < time_quantum; i++){
				p->Run(1);

				cout << "System["<< (time-2) << "]........." << "Process[PID=" << p->getPid() << "] is running\n"; 
				time++;
				if(p->is_Completed()){
					break;
				}
			}

			if(p->is_Completed()){
				response_time += (time-2) - p->get_arrival_time();
				wait_time += (time-2) - p->get_arrival_time() - p->get_cpu_burst_time();
				cout << "System["<< (time-2) << "]........." << "Process[PID=" << p->getPid() << "] is finished its job!\n";
				//cout << "Response Time: " <<  response_time << "\n";
				//cout << "Wait Time: " <<  wait_time << "\n";
				processVec.erase(processVec.begin()+i);
			}
			else
				i++;
		}

	std::cout << "Average waiting time seconds: " <<  wait_time/process_info.size() << "\n";
	std::cout << "Average response time seconds: " << response_time/process_info.size() << "\n";
}

//Fill in the procesVec with shared_ptr
//And set time_quantum, or you can also use set_time_quantum function for setting quantum
RoundRobin::RoundRobin(vector<ProcessInfo> &process_info, int time_quantum){

	for(auto it : process_info){
			Process p(get<0>(it), get<1>(it), get<2>(it));
			auto proc = make_shared<Process>(p);
			processVec.push_back(proc);
	}

	set_time_quantum(time_quantum);


}

void RoundRobin::print(){
	for (auto ele : processVec){
		cout << ele->getPid() << "; " << ele->get_arrival_time() << "; " << ele->get_cpu_burst_time() << endl;
	}
}

