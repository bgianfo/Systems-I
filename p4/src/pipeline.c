/*
** Authors:
** Brian Gianforcaro (bjg1955@cs.rit.edu)
** Nicholas Williams (nxw9676@cs.rit.edu)
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>

#include "pipeline.h"

int main( size_t argc, char** argv ) {

  char string[] = "Hello, world!\n";
  char readbuffer[80];

  printf(" %s\n", argv[1] );
  /* Just exit if no arguments */
  if ( argc < 2 ) {
    exit( EXIT_SUCCESS );
  }


  bool redirect_in = false;
  bool redirect_out = false;

  char* ofile;
  char* ifile;

  for ( int i = 1; i < argc; i++ ) {
    if ( '<' == argv[i] ) {
      redirect_in = true;
      if ( (i+1) < argc ) {
        ifile = argv[i+1];
      }
    } else if ( '>' == argv[i] ) {
      redirect_out = true;
      if ( (i+1) < argc ) {
        ofile = argv[i+1];
      }
    }
  }

  int fd[2];
  pipe(fd);

  /*
  ** TODO/Hints:
  **  - Loop to continually process arguments.
  **  - Might need to call dup()/dup2() to allows access to parent descriptors in child.
  **  - Uses  exec/execlp and friends to actuall call process.
  **  - File handeling shit.
  */

  pid_t  childpid;
  if( (childpid = fork()) == FORK_ERR ) {
    perror("fork");
    _exit( EXIT_FAILURE );
  }

  if ( childpid == 0 ) {
    /* Child process closes up input side of pipe */
    close( fd[IN] );

    /* Send "string" through the output side of pipe */
    write( fd[OUT], string, (strlen(string)+1) );
    exit( EXIT_SUCCESS );
  } else {
    /* Parent process closes up output side of pipe */
    close( fd[OUT] );

    /* Read in a string from the pipe */
    int nbytes = read( fd[IN], readbuffer, sizeof(readbuffer) );
    printf( "Received string: %s", readbuffer );
  }

  return EXIT_SUCCESS;
}

