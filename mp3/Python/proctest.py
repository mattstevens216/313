#!/usr/bin/env python

# --------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                   #
# Project: CSCE-313 Machine Problem #3                                       #
#                                                                             #
# File: ./student/proctest.py                                                 #
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# Standard Library Includes                                                   #
# --------------------------------------------------------------------------- #

import sys
import re
from subprocess import check_output, CalledProcessError


"""
Note to students: Your program must be written using the provided class
template below.  Functions that require your implementation are marked
with "Work needed here" in their method docstrings.

Be sure to delete all of these extraneous comments before submitting your
code for grading.  Also, delete all extra 'pass' statements.  They are
noop instructions and only there because Python does not like empty functions.
"""


class Proctest(object):
    """
    Object to contain process data and getter functions
    """

    def __init__(self, pid):
        """
        Read all data from pertinent files and save as class members

        Work needed here!
        """
        pass


    def getpid(self):
        """
        Returns the process id

        Work needed here!
        """
        pass


    def getppid(self):
        """
        Returns process's parent's ID

        Work needed here!
        """
        pass


    def geteuid(self):
        """
        Returns the process's effective user ID

        Work needed here!
        """
        pass


    def getegid(self):
        """
        Returns process's effective group ID

        Work needed here!
        """
        pass


    def getruid(self):
        """
        Returns process's real user ID

        Work needed here!
        """
        pass


    def getrgid(self):
        """
        Returns process's real group ID

        Work needed here!
        """
        pass


    def getfsuid(self):
        """
        Returns process's file system user ID

        Work needed here!
        """
        pass


    def getfsgid(self):
        """
        Returns process's file system group ID

        Work needed here!
        """
        pass


    def getstate(self):
        """
        Returns process's current state

        Work needed here!
        """
        pass


    def getthread_count(self):
        """
        Returns process's current thread count

        Work needed here!
        """
        pass


    def getpriority(self):
        """
        Returns process's current prioroty number

        Work needed here!
        """
        pass


    def getniceness(self):
        """
        Returns process's current niceness value

        Work needed here!
        """
        pass


    def getstime(self):
        """
        Returns process's current stime value

        Work needed here!
        """
        pass


    def getutime(self):
        """
        Returns process's current utime value

        Work needed here!
        """
        pass


    def getcstime(self):
        """
        Returns process's current cstime value

        Work needed here!
        """
        pass


    def getcutime(self):
        """
        Returns process's current cutime value

        Work needed here!
        """
        pass


    def getstartcode(self):
        """
        Returns process's startcode

        Work needed here!
        """
        pass


    def getendcode(self):
        """
        Returns process's endcode

        Work needed here!
        """
        pass


    def getesp(self):
        """
        Returns process's stack pointer

        Work needed here!
        """
        pass


    def geteip(self):
        """
        Returns process's instruction pointer

        Work needed here!
        """
        pass


    def getfiles(self):
        """
        Returns process's current number of open file descriptors

        Work needed here!
        """
        pass


    def getvoluntary_context_switches(self):
        """
        Returns the number of times that the process has voluntarily
        yielded control to the kernel.

        Work needed here!
        """
        pass


    def getnonvoluntary_context_switches(self):
        """
        Returns the number of times that the process has had control
        taken from it forcefully by the kernel.

        Work needed here!
        """
        pass


    def getlast_cpu(self):
        """
        Returns the last cpu that the process executed on

        Work needed here!
        """
        pass


    def getallowed_cpus(self):
        """
        Returns a list of allowed processors

        Work needed here!
        """
        pass


    def getmemory_map(self):
        """
        Returns process's memory map

        Work needed here!
        """
        pass

def main():

    # Read in PID
    sys.stdout.write("Enter the PID of a process: ")
    sys.stdout.flush()
    process_pid = sys.stdin.readline()

    process_data = Proctest(process_pid)

    # Start printing out values
    print ""
    print "Process Information:"
    print "  1) Identifiers"
    print "     PID: %s" % process_data.getpid()
    print "     PPID: %s" % process_data.getppid()
    print "     EUID: %s" % process_data.geteuid()
    print "     EGID: %s" % process_data.getegid()
    print "     RUID: %s" % process_data.getruid()
    print "     RGID: %s" % process_data.getrgid()
    print "     FSUID: %s" % process_data.getfsuid()
    print "     FSGID: %s" % process_data.getfsgid()
    print ""
    print "  2) State"
    print "     State: %s" % process_data.getstate()
    print ""
    print "  3) Thread Information"
    print "     Thread Count: %s" % process_data.getthread_count()
    print ""
    print "  4) Priority"
    print "     Priority Number: %s" % process_data.getpriority()
    print "     Niceness Value: %s" % process_data.getniceness()
    print ""
    print "  5) Time Information"
    print "     stime: %s" % process_data.getstime()
    print "     utime: %s" % process_data.getutime()
    print "     cstime: %s" % process_data.getcstime()
    print "     cutime: %s" % process_data.getcutime()
    print ""
    print "  6) Address Space"
    print "     Startcode: %s" % process_data.getstartcode()
    print "     Endcode: %s" % process_data.getendcode()
    print "     ESP: %s" % process_data.getesp()
    print "     EIP: %s" % process_data.geteip()
    print ""
    print "  7) Resources"
    print "     File Handles: %s" % process_data.getfiles()
    print "     Voluntary Context Switches: %s" % process_data.getvoluntary_context_switches()
    print "     Nonvoluntary Context Switches: %s" % process_data.getnonvoluntary_context_switches()
    print ""
    print "  8) Processor"
    print "     Last Processor: %s" % process_data.getlast_cpu()
    print "     Allowed Cores: %s" % process_data.getallowed_cpus()
    print ""
    print "  9) Memory Map"
    temp_mem_array = process_data.getmemory_map()
    for line in temp_mem_array:
        print "     %s" % line


if __name__ == "__main__":
    main()

