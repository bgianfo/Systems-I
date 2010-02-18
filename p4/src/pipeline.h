/*
 * File:  pipeline.h
 *
** Authors:
**    Brian Gianforcaro (bjg1955@cs.rit.edu)
**    Nicholas Williams (nxw6572@cs.rit.edu)
**
** Description:
**    Description
**
*/

#ifndef _PIPELINE_H_
#define _PIPELINE_H_

/* Process input and output indices to the fd array */
#define IN        0
#define OUT       1

#define FORK_ERR -1

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>

/*
** Illegal null command
** No command given on one side of a pipe symbol
*/

/* 
** name: Illegal input redirect
** Error in attempted input redirect of name (not the first command, not the first input redirect, etc.
*/ 

/*
** name: Illegal output redirect
** Error in attempted output redirect of name (not the last command, not the first output redirect, etc.
**
*/

/* 
** name: reason
** Error message printed by perror(name) after an open(name) failed 
*/

#endif
