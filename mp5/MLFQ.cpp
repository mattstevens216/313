#include "MLFQ.h"

//The goal of this function is to fill the 4 levels with process_info 
//You should sort the process_info by the arrival_time of the Process;
//Process with smaller arrival time will have smaller index in the vector

//After filling in the top 3 levels, then do insertion for the fcfs level
MLFQ::MLFQ(string file)
{
	int queueSelection = 0;
	int numUpperLevels = 3;
	extractProcessInfo(file);
	for(int i = 0; i < numUpperLevels; i++){
		upperLevels.push_back(vector<shared_ptr<Process>>()); //initialize each of the levels.
	}

	std::sort(begin(process_info), end(process_info), [](ProcessInfo const &t1, ProcessInfo const &t2) {
		return get<1>(t1) < get<1>(t2);
	});

	for(auto ele : process_info){
		Process p = Process(get<0>(ele),get<1>(ele), get<2>(ele));
		if(queueSelection == 0){
			upperLevels.at(queueSelection).push_back(make_shared<Process>(p));
			if(upperLevels.at(queueSelection).size() == LEVEL1_CAPACITY)
				queueSelection++; //if this level is full go to next level
		}else if(queueSelection == 1){
			upperLevels.at(queueSelection).push_back(make_shared<Process>(p));
			if(upperLevels.at(queueSelection).size() == LEVEL2_CAPACITY)
				upperLevels.at(queueSelection).push_back(make_shared<Process>(p));
		}
		else if(queueSelection == 2){
			upperLevels.at(queueSelection).push_back(make_shared<Process>(p));
			if(upperLevels.at(queueSelection).size() == LEVEL3_CAPACITY)
				upperLevels.at(queueSelection).push_back(make_shared<Process>(p));
		}else{
			lowestLevel.push(p);
			update_last_process_fcfs(p);
		}
	} 	
}
//This function is used to keep track of the process who entered the fcfs queue most recently
//The purpose is to properly adjust the upcoming process's arrival time
//If the arrival time is changed, you can still check its original arrival time by accessing the 
//process_info variable
void MLFQ::update_last_process_fcfs(Process process_to_fcfs) {
	last_process_fcfs = process_to_fcfs;
}

//This function has the following assumptions:
/*
	1. The start_level is not equal goal_level;
	2. goal_level is larger then start_level
	3. When you use this function, you should know the process should not jump from level 0 to level 2 or 3 if the level 1 has a space there.
	   Generally, when you degrade a process, it tries to go to the level below by one level and if that level is full, it will keep going down 
	   until it finds a level which has space there.
	4. Successful jump will return 1, else 0
	5. To successfully jump to the goal_level, the process must go to the end of the vector corresponding to goal_level
*/
//start_level is the level the process is located at, it is one value of 0 , 1, 2;
//pos is its index in the vector
//goal_level is the level it tries to enter
int MLFQ::level_jump(shared_ptr<Process> p, unsigned int start_level, unsigned int pos, unsigned int goal_level) {
	try{
		if(goal_level < 3){
			upperLevels.at(goal_level).push_back(make_shared<Process>(*p));
		}else{
			p->change_arrival_time((last_process_fcfs.get_arrival_time()+1));
			lowestLevel.push(*p);
			update_last_process_fcfs(*p);
		}
		upperLevels.at(start_level).erase(upperLevels.at(start_level).begin() + pos);
		return 1;
	} catch(...){
		cout << "Error at level_jump" << endl;
		return 0; //catch anything return from function
	}
}


/*
 p is the process which is going to be degrade to lower levle
 levle is the level it is located currently
 legal value of level can be: 0, 1, 2 Since no need to degrade for last level
 pos is the its index in the vector

0: it is located at the top level
1:  it is located at the second highest level
2:  it is located at the third highest level
3: it is in the fcfs level, no need to degrade, it will stay there until finishing the job and leave
*/

//pos is the index of the process in the vector
//Your goal is to degrade this process by one level
//You can use level_jump() function here based on which level the process is going to jump
void MLFQ::degrade_process(shared_ptr<Process> p, unsigned int level, unsigned int pos) {
	if(level == 0){
		if(upperLevels.at(level+1).size() < LEVEL2_CAPACITY) //if room on level 2, add to level 2.
			level_jump(p, level, pos, 1);	
		else if(upperLevels.at(level+2).size() < LEVEL3_CAPACITY) //if room on level 3, add to level 3.
			level_jump(p, level, pos, 2);
		else
			level_jump(p, level, pos, 3);
	}
	else if(level == 1){
		if(upperLevels.at(level+1).size() < LEVEL3_CAPACITY)
			level_jump(p, level, pos, 2);	
		else
			level_jump(p, level, pos, 3);
	}
	else if (level == 2){
		level_jump(p, level, pos, 3);
	} else {
		cout << "Error at level function. " << endl;
	}	
}

/*
You can use multiple loops here to do the job based on the document;
Make sure print out the timing information correctly
*/
void MLFQ::schedule_tasks(){
	int time = 2; //set to 2 because of the print statement in the pdf starts time 0 but executes pid = 1 when arrival time = 2
	int wait_time = 0;
	int response_time = 0;

	while(!(upperLevels.at(0).empty() && lowestLevel.empty())){
		cout << "System Time[" << time << "].........First Level Queue Starts to Work." << endl;
		while(!(upperLevels.at(0).empty())){ //if the first level isnt empty
			auto elem = upperLevels.at(0).at(0); //start at the first element of the first level
			cout << "System Time[" << time-2 << "].........Process[PID=" << elem->getPid() << "] is running." << endl;
			for(int i = 0; i < LEVEL1_QUANTUM; i++){ //Run all processes in the level 1 queue.
				cout << "System Time[" << time-2 << "].........Process[PID=" << elem->getPid() << "] is runnning." << endl;
				elem->Run(1);
				time += 1;
				if(elem->is_Completed()){
					response_time += time;
					wait_time += time - (elem->get_arrival_time() + elem->get_cpu_burst_time());
					cout << "System Time[" << time-2 << "].........Process[PID=" << elem->getPid() << "] has finished." << endl;
				}
			}
			if(elem->is_Completed()){ //remove the first element of the first level
					upperLevels.at(0).erase(upperLevels.at(0).begin());   
			}
			else {
				cout << "Degrading [PID=" << elem->getPid() << "]" << endl;
				degrade_process(elem, 0, 0); //process, level, position
			}
		}
		cout << "System Time[" << time << "].........second level queue starts to work." << endl;	
		while(!(upperLevels.at(1).empty())){ //if level 2 is not empty
			auto elem = upperLevels.at(1).at(0); //start at the first element of the second level
			cout << "System Time[" << time-2 << "].........Process[PID=" << elem->getPid() << "] is running." << endl;
			for(int i = 0; i < LEVEL2_QUANTUM; i++){ //run all the processes in level 2
				cout << "System Time[" << time-2 << "].........Process[PID=" << elem->getPid() << "] is runnning." << endl;
				elem->Run(1);
				time += 1;
				if(elem->is_Completed()){
					response_time += time;
					wait_time += time - (elem->get_arrival_time() + elem->get_cpu_burst_time());
					cout << "System Time[" << time << "].........Process[PID=" << elem->getPid() << "] has finished." << endl;
				}
			}
			if(elem->is_Completed())
				upperLevels.at(1).erase(upperLevels.at(1).begin()); //level 2 remove first element
			else{
				cout << "System Time[" << time-2 << "].........Degrading Process [PID=" << elem->getPid() << "]" << endl;
				degrade_process(elem, 1, 0); //process, level, position
			}
		}
		cout << "System Time[" << time << "].........third level queue starts to work." << endl;	
		while(!(upperLevels.at(2).empty())){ // if the third level is empty
			auto elem = upperLevels.at(2).at(0); //start at the first element of the third level
			cout << "System Time[" << time-2 << "].........Process[PID=" << elem->getPid() << "] is running." << endl;
			for(int i = 0; i < LEVEL3_QUANTUM; i++){ //run all the processes in the 3rd level
				cout << "System Time[" << time-2 << "].........Process[PID=" << elem->getPid() << "] is running." << endl;
				elem->Run(1);
				time += 1;
				if(elem->is_Completed()){
					response_time += time;
					wait_time += time - (elem->get_arrival_time() + elem->get_cpu_burst_time());
					cout << "System Time[" << time-2 << "].........Process[PID=" << elem->getPid() << "] has finished." << endl;
				}

			}
			if(elem->is_Completed())
				upperLevels.at(2).erase(upperLevels.at(2).begin()); //erase the first element of the third level
			else 
				degrade_process(elem, 2, 0); //process, level, position
		}
		cout << "System Time[" << time << "].........FCFS Queue starts to work." << endl;	
		for(int i = 0; i < FCFS_WORKLOAD; i++){
			if(!(lowestLevel.empty())){
				auto elem = lowestLevel.top(); //dont forget lowest level is a priority queue based on arrival time
				cout << "System Time[" << time-2 << "].........Process[PID=" << elem.getPid() << "] starts running.";
				while(!(elem.is_Completed())){	//executes until the current process finishes running
					cout << "System Time[" << time << "].........Process[PID=" << elem.getPid() << "] is running." << endl;
					elem.Run(1);
					time += 1;
				} 
				//remove from queue
				cout << "System Time[" << time-2 << "].........Process[PID=" << elem.getPid() << "] has finished." << endl;
				response_time += time - elem.get_initial_arrival_time();
				wait_time += time - (elem.get_initial_arrival_time() + elem.get_cpu_burst_time());
				lowestLevel.pop();
			}
		}
	}
	printf("Average wait time %.3f seconds\n", (((float) wait_time) / process_info.size()));
	printf("Average response time %.3f seconds\n", (((float) response_time) / process_info.size()));
}
