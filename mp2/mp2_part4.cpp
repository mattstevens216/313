/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part4.cpp                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Standard Library Includes                                                  */
/* -------------------------------------------------------------------------- */

#include <stdlib.h>
#include <unistd.h>


int main()
{
    char*argList[] = {"ls", "-la"};
    execvp(argList[0], argList);
    exit(0);
}
