/*
** Brian Gianforcaro (bjg1955@cs.rit.edu)
**
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

#include <pipeline.h>

int main(size_t argc, char** argv ) {

  char string[] = "Hello, world!\n";
  char readbuffer[80];

  /* Just exit if no arguments */
  if ( argc < 2 ) {
    exit( EXIT_SUCCESS );
  }

  int fd[2];
  pipe(fd);

  pid_t  childpid;
  if( (childpid = fork()) == FORK_ERR ) {
    perror("fork");
    exit( EXIT_FAILURE );
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


