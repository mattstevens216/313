/* ------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                 */ 
/* Project: CSCE-313 Machine Problem #4                                              */
/*                                                                           */
/* File: ./shell.cpp                                                         */ 
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Standard Libaray Includes                                                 */
/* ------------------------------------------------------------------------- */

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<unistd.h>
#include<stdlib.h>
#include<regex>
#include<string>
#include<vector>
#include<stdio.h>
#include<string.h>
#include<string>
#include<sstream>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "shell.h"
/* ------------------------------------------------------------------------- */
/* Global Variables                                                          */
/* ------------------------------------------------------------------------- */

// Put any global variables here

/* User Definable Custom Prompt:
 * 
 * Allows users to define custom prompts using a simple command to change the string.  
 * The following set of macros will be replaced as follows:
 *
 *   - DIR: Current directory 
 *   - WHO: User's name
 *   - DATE: Current date
 *   - TIME: Current time
 */
shell::shell(){}

void shell::update_prompt()
{
/* 	DIR = .;
	WHO = x.username;
	DATE = x.date;
	TIME = X.time; */
}

void shell::parse_Commands(int argc, char** argv){
	for(int i = 1; i < argc; ++i){ //starting at 1 because we do not need the program name
		Shell.commands[i] = argv[i];
		std::cout << Shell.commands[i];
	}
	
} 

void shell::tokenize_Commands()
void shell::run(){
	parse_Commands(argc, argv);
	initializePrompt();
	
}



int main(int argc, char** argv)
{
	/* Parse command line arguments */
	shell.run();
	/* Initialize prompt to default (current directory followed by a colon */

	/* Vector to maintain background processes */

	/* Tokenize input command */

	// The tokenizer may make empty tokens, eliminate them

	// Account for special commands

	// Check to see if the process is to run in the background or foreground

	/* Remove quotes from beginning and end of tokens */

	/* Replace the string \" with the character '\"' */

	/* Detect command pipes */

	/* Set up communication pipes */

	/* Detect Redirection */

	/* Execute commands in order */

	return 0;
}
