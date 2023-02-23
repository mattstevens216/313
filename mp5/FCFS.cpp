//Jason Alonzo & Matthew Stevens

#include "FCFS.h"
#include <stdlib.h>
//Use Base class's function to extract process information from file
//And fill in the fcfs_queue; the priority in this queue is the arrival time; 
//Make sure you did the ArrivalEarlierThan Comparator
FCFS::FCFS(string file)
{
	Scheduler::extractProcessInfo(file);
	
	for(int i = 0; i < process_info.size(); i++){
		int pid = get<0>(process_info[i]);
		int arrival_time = get<1>(process_info[i]);
		int cpu_burst_time = get<2>(process_info[i]);
		fcfs_queue.push({pid, arrival_time, cpu_burst_time});
	}
}

void FCFS::print(){
	cout<<"pid\t"<<"\tarrival time\t"<<"\tburst time:\n";
	for(auto ele:process_info){
		cout<<get<0>(ele)<<"\t"<<get<1>(ele)<<"\t"<<get<2>(ele)<<endl;
	}
}

//Assuming the time the CPU starts working with process is system time 0
void FCFS::schedule_tasks(){
	int time = 0;
	float wait_time = 0;
	float response_time = 0;
	float total_wait = 0;
  	while( !(fcfs_queue.empty()) ){
		auto const& iter = fcfs_queue.top();
		Process proc = Process(iter.getPid(), iter.get_arrival_time(), iter.get_cpu_burst_time());
		while(!(proc.is_Completed())){
			std::cout << "System Time[" << time << "].......Process[" << proc.getPid() << "] is Running." << endl;
			proc.Run(1);
			time += 1;
			wait_time = 0;
		}
		if(proc.is_Completed()){
			response_time += time - proc.get_arrival_time();
			wait_time += time - proc.get_arrival_time() - proc.get_cpu_burst_time();
			//cout << "Response Time: " <<  response_time << "\n";
			//cout << "Wait Time: " <<  wait_time << "\n";
			std::cout << "System Time[" << time << "].......Process[" << proc.getPid() << "] finished its job." << endl;
			fcfs_queue.pop();

			total_wait += wait_time;
		} 
	}

	cout << "Average waiting time seconds: " <<  total_wait/process_info.size() << "\n";
	cout << "Average response time seconds: " << response_time/process_info.size() << "\n";
}

FCFS::FCFS(){}
