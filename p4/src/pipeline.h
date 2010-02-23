/*
** File:  pipeline.h
**
** Authors:
**    Brian Gianforcaro (bjg1955@cs.rit.edu)
**    Nicholas Williams (nxw6572@cs.rit.edu)
**
** Description:
**    Defines includes, constants, and prototypes for pipeline.c
**
*/

#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#define ERROR  -1

#define dup3( x, y )  if ( dup2( x , y) == ERROR ) { \
                          perror("dup2"); \
                           _exit( EXIT_FAILURE ); \
                      }

#define close2( x )  if ( close( x ) == ERROR ) { \
                          perror("close"); \
                           _exit( EXIT_FAILURE ); \
                    }


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

/*
** sets up the next command (args) for execvp in child process
*/
bool next_cmd( char*** argv, char*** args, int *argc );

/*
** recursive method for setting up fork() with multiple commands
*/
void do_child( char** argv, char** args, int argc, bool last_cmd );

#endif
