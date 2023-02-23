#include "Scheduler.h"
#include <iostream>

//Read a process file to extract process information
//All content goes to proces_info vector
void Scheduler::extractProcessInfo(string file){
	string line;
	ProcessInfo processArgs;
	int filePid;
	int fileArrival;
	int fileBurst;
	ifstream inputFile(file);
	if( inputFile.is_open()){
		while(inputFile >> filePid >> fileArrival >> fileBurst){
			processArgs = make_tuple(filePid, fileArrival, fileBurst);
			process_info.push_back(processArgs);
		//while(!(inputFile.eof())){
			/*inputFile >> filePid >> fileArrival >> fileBurst; //read in next 3 characters and store in respective variables.
			get<0>(processArgs) = filePid;
			get<1>(processArgs) = fileArrival;
			get<2>(processArgs) = fileBurst; 
			process_info.push_back(processArgs); //push_back a tuple into process_info vector
		}*/
		}
	}
	inputFile.close();
}
