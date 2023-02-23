#include "SRTF.h"

void SRTF::printByArrivalTime(){
	cout << "pid   " << "  arrival time     " << "     burst time:\n";
	for (auto ele : process_info){
		cout << get<0>(ele) << "\t" << get<1>(ele) << "\t" << get<2>(ele) << endl;
	}
}

SRTF::SRTF(string file) //construct then fill with processes from process_info.
{
	extractProcessInfo(file);

	std::sort(begin(process_info), end(process_info), [](ProcessInfo const &t1, ProcessInfo const &t2) {
		return get<1>(t1) < get<1>(t2);
	});

	for(int i = 0; i < process_info.size(); i++){
		int pid = get<0>(process_info[i]);
		int arrival_time = get<1>(process_info[i]);
		int cpu_burst_time = get<2>(process_info[i]);
		Process proc = Process(pid, arrival_time, cpu_burst_time);
		SRTF_queue.push(make_shared<Process>(proc));
	}
	
}
//scheduling implementation
void SRTF::schedule_tasks(){
	/*
	Fill in this part
	*/
	int wait_time = 0;
	int response_time = 0;
	int time = 0;
	int pidArrivalTime = 0;

	while(!(SRTF_queue.empty())){	
		auto const& iter = SRTF_queue.top();
		if(iter -> get_arrival_time() <= time){
			shared_ptr<Process> p1 (new Process(iter->getPid(), iter->get_arrival_time(), iter->get_cpu_burst_time()));
			while(!(p1->is_Completed())){
				std::cout << "System Time[" << time << "].......Process[PID=" << p1->getPid() << "] is Running." << endl;
				p1->Run(1);
				time += 1;
			}
			if(p1->is_Completed()){
					std::cout << "System Time[" << time << "].......Process[PID=" << p1->getPid() << "] finished its job." << endl;
					response_time += time - (p1->get_initial_arrival_time());
					wait_time += time - (p1->get_initial_arrival_time() + p1->get_cpu_burst_time());
					SRTF_queue.pop();
			}
		} else 
			time += 1;
	}
	printf("The average wait time %.3f seconds. \n", (((float) wait_time) / process_info.size())); // wait time of the element / total number of elements
	printf("The average response time %.3f seconds. \n", (((float) response_time) / process_info.size())); // response time of element / total number of elements
}
