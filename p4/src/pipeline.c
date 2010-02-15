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

bool rw_to( char* filename, int fd ) {

  char buffer[200];
  FILE* file = fdopen( filename, "r" );

  if ( NULL == file ) {
    perror("file");
    return false;
  }

  while ( !feof(file) ) {
   int nread = fgets( buffer, 200, file );
   write( fd, buffer, nread );
  }
  return true;
}

int main( size_t argc, char** argv ) {

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

  /*
  ** TODO/Hints:
  **  - Loop to continually process arguments.
  **  - Might need to call dup()/dup2() to allows access to parent descriptors in child.
  **  - Uses  exec/execlp and friends to actuall call process.
  **  - File handeling shit.
  */

  int cur = 1;
  while( true ) {

    int fd[2];
    if ( pipe(fd) == -1 ) {
      // Errorr!
    }

    pid_t childpid = fork();

    if ( childpid == FORK_ERR ) {
      perror("fork");
      _exit( EXIT_FAILURE );
    }


    if ( childpid == 0 ) {

      if ( /* we are getting in put on left */ ) {
        dup2( fds[IN], STDIN_FILENO );
        close( fds[IN] );
      } else if ( /* we are sending output on right */  ) {
        dup2( fds[OUT], STDOUT_FILENO );
        close( fds[OUT] );
      }

      if ( redirect_in ) { 
        rw_to( ifile, stdout );
        redirect_in = false;
      }

      char *const args[] = { sysargv[cur] , NULL };
      execcv( args[0], args );
      _exit( EXIT_FAILURE );

    } else {

    }

    close( fd[IN] );
    close( fd[OUT] );

    wait( NULL );
    wait( NULL );

  }


  return EXIT_SUCCESS;
}
