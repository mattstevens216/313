#!/bin/bash

# --------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                   #
# Project: CSCE-313 Machine Problem #3                                        #
#                                                                             #
# File: ./proctest.sh                                                         #
# --------------------------------------------------------------------------- #

# Forward declare the PID variable
PID=""

# Need to filter program so that only the functions are defined on a test
TESTING=false
if [ $# -gt 0 ]
then
    TESTING=true
fi

function getpid()
{
	OUTPUT=$( grep '^Pid' /proc/$PID/status | awk '{ print $2 }' )
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: getpid does not work"
    fi
}

function getppid()
{
	OUTPUT=$( grep '^PPid' /proc/$PID/status | awk '{ print $2 }' ) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: ppid does not work"
    fi
}

function geteuid()
{
	OUTPUT=$( grep '^Uid' /proc/$PID/status | awk '{ print $3 }' ) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: euid does not work"
    fi
}

function getegid()
{
	OUTPUT=$( grep '^Gid' /proc/$PID/status | awk '{ print $3 }' ) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: egid does not work"
    fi
}

function getruid()
{
	OUTPUT=$( grep '^Uid' /proc/$PID/status | awk '{ print $2 }' ) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: ruid does not work."
    fi
}

function getrgid()
{
	OUTPUT=$( grep '^Gid' /proc/$PID/status | awk '{ print $2 }' ) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: rgid does not work"
    fi
}

function getfsuid()
{
	OUTPUT=$( grep '^Uid' /proc/$PID/status | awk '{ print $5 }' ) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: fsuid does not work"
    fi
}

function getfsgid()
{
	OUTPUT=$( grep '^Gid' /proc/$PID/status | awk '{ print $5 }' ) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: fsgid does not work"
    fi
}

function getstate()
{
	OUTPUT=$( grep '^State' /proc/$PID/status | awk '{ print $2 }' ) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: state does not work"
    fi
}

function getthread_count()
{
	OUTPUT=$( awk '{ print $20 }' /proc/$PID/stat)  
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: thread count does not work"
    fi
}

function getpriority()
{
	OUTPUT=$( awk '{ print $18 }' /proc/$PID/stat) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: priority does not work"
    fi
}

function getniceness()
{
	OUTPUT=$( awk '{ print $19 }' /proc/$PID/stat) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: niceness does not work"
    fi
}

function getstime()
{
	OUTPUT=$( awk '{ print $15 }' /proc/$PID/stat) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: stime does not work."
    fi
}

function getutime()
{
	OUTPUT=$( awk '{ print $14 }' /proc/$PID/stat) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: utime does not work."
    fi
}

function getcstime()
{
	OUTPUT=$( awk '{ print $17 }' /proc/$PID/stat) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: cstime does not work"
    fi
}

function getcutime()
{
	OUTPUT=$( awk '{ print $16 }' /proc/$PID/stat) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: cutime does not work"
    fi
}

function getstartcode()
{
	OUTPUT=$( awk '{ print $26 }' /proc/$PID/stat) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: startcode does not work"
    fi
}

function getendcode()
{
	OUTPUT=$( awk '{ print $27 }' /proc/$PID/stat) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: endcode does not work."
    fi
}

function getesp()
{
	OUTPUT=$( awk '{ print $29 }' /proc/$PID/stat) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: esp does not work."
    fi
}

function geteip()
{ 
	OUTPUT=$( awk '{ print $30 }' /proc/$PID/stat) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: eip does not work."
    fi
}

function getfiles()
{
    if [ ! -r "/proc/$PID/fd" ]
    then
        echo "Permission denied"
    else
        OUTPUT=$(ls -la /proc/$PID/fd | wc -l)
        if [ $? -eq 0 ] 
        then 
            OUTPUT=$(($OUTPUT - 3))
            echo $OUTPUT
        else
            echo "error: files does not work."
        fi
    fi
}

function getvoluntary_context_switches()
{
	OUTPUT=$( grep '^voluntary_ctxt_switches' /proc/$PID/status | awk '{ print $2 }' ) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: vol context switch does not work"
    fi
}

function getnonvoluntary_context_switches()
{
	OUTPUT=$( grep '^nonvoluntary_ctxt_switches' /proc/$PID/status | awk '{ print $2 }' ) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: nonvol context switch does not work."
    fi
}

function getlast_cpu()
{
	OUTPUT=$( awk '{ print $39 }' /proc/$PID/stat) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: last_cpu does not work."
    fi
}

function getallowed_cpus()
{
	OUTPUT=$( grep '^Cpus_allowed_list' /proc/$PID/status | awk '{ print $2 }' ) 
    if [ $? -eq 0 ]
    then
        echo $OUTPUT
    else 
        echo "error: allowed_cpus does not work."
    fi
}

function getmemory_map()
{
    cat /proc/$PID/maps 1> /dev/null 2> /dev/null
    if [ $? -ne 0 ]
    then
        echo "error: unable to access that directory. We need permission."
    else
        cat /proc/$PID/maps
    fi
}

# Main program here
if [ $TESTING == false ]
then
    # Read in the value of the pid that the user would like to examine.  
    printf "Enter the pid of a process: " 
    read PID

    # If the value entered is not an integer, try again.  
    while ! [ "$PID" -eq "$PID" ] 2>/dev/null
    do
        printf "[ERROR]: Number was not a valid integer.  Try again: " 
        read PID
    done

    # Search the system to make sure that the process exists.  
    ls "/proc/$PID" > /dev/null
    if [ $? == 1 ]
    then
        echo "[ERROR]: A Process with the given ID does not exist on this system! - Exiting!"
        exit 1
    fi

    printf "\nProcess Information: " 
    
    printf "\n1) Identifiers\n"
    printf "   PID:   $(getpid) \n"
    printf "   PPID:  $(getppid) \n"
    printf "   EUID:  $(geteuid) \n"
    printf "   EGID:  $(getegid) \n"
    printf "   RUID:  $(getruid) \n"
    printf "   RGID:  $(getrgid) \n"
    printf "   FSUID: $(getfsuid) \n"
    printf "   FSGID: $(getfsgid) \n"
    
    printf "\n2) State: \n"
    printf "   State: $(getstate)\n"
    
    printf "\n3) Thread Information: \n"
    printf "   Thread Count: $(getthread_count) \n"
    
    printf "\n4) Priority: \n"
    printf "   Priority Number: $(getpriority) \n"
    printf "   Niceness Value:  $(getniceness) \n"
    
    printf "\n5) Time Information: \n"
    printf "   stime:  $(getstime) \n"
    printf "   utime:  $(getutime) \n"
    printf "   cstime: $(getcstime) \n"
    printf "   cutime: $(getcutime) \n"
    
    printf "\n6) Address Space: \n"
    printf "   Startcode: $(getstartcode) \n"
    printf "   Endcode:   $(getendcode) \n"
    printf "   ESP:       $(getesp) \n"
    printf "   EIP:       $(geteip) \n"
    
    printf "\n7) Resourses: \n"
    printf "   File Handles: $(getfiles) \n"
    printf "   Voluntary Context Switches: $(getvoluntary_context_switches)\n"
    printf "   Involuntary Context Switches: $(getnonvoluntary_context_switches)\n"
    
    printf "\n8) Processors: \n"
    printf "   Last processor: $(getlast_cpu) \n"
    printf "   Allowed Cores:  $(getallowed_cpus) \n"
        
    printf "\n9) Memory Map: \n"
    printf "$(getmemory_map) \n"

    exit 0
fi
