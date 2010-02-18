/*
** File:  pipeline.c
** 
** Authors:
**    Brian Gianforcaro (bjg1955@cs.rit.edu)
**    Nicholas Williams (nxw6572@cs.rit.edu)
**
** Description:
**    Description
**
*/

#include "pipeline.h"

/*
bool rw_to( char* filename, int fd ) {

  char buffer[ 200 ];
  FILE* file = fopen( filename, "r" );

  if ( file == NULL ) {
    perror( "file" );
    return ( false );
  }

  while ( !feof( file ) ) {
   fgets( buffer, 200, file );
   write( fd, buffer, 200 );
  }

  return ( true );
}
*/

int main( int argc, char* argv[] ) {

  /* Just exit if no arguments */
  if ( argc < 2 ) {
    exit( EXIT_SUCCESS );
  } 

  /*
  ** Check if the first or last command are pipes;
  ** if so print out an error message and exit
  */ 
  if ( !strcmp( argv[ 1 ], "|" ) || !strcmp( argv[ argc - 1 ], "|" ) ) {
    fprintf( stderr, "Illegal null command\n" );
    exit( EXIT_FAILURE );
  }

  bool redirect_in = false;
  bool redirect_out = false;

  char* ofile;
  char* ifile;

  /*
  ** Loop through arguments and check for illegal
  ** null commands before/after pipe, illegal input
  ** redirects and illegal output redirects
  **/
  bool block_in = false;
  for ( int i = 2; i < argc - 1; i++ ) {

    if ( !strcmp( argv[ i ], "|" ) ) {
      
      if ( !strcmp( argv[ i - 1 ], "|" ) || !strcmp( argv[ i + 1 ], "|" ) ) {
        fprintf( stderr, "Illegal null command\n" );
        exit( EXIT_FAILURE );
      }
      
      if ( redirect_out ) {
        fprintf( stderr, "%s: Illegal output redirect\n", argv[ i - 1 ] );
        exit( EXIT_FAILURE );
      }
    
      block_in = true;
    
    } else if ( !strcmp( argv[ i ], "<" ) ) {
      
      if ( block_in || redirect_in ) {
        fprintf( stderr, "%s: Illegal input redirect\n", argv[ i + 1 ] );
        exit( EXIT_FAILURE );
      } 
      
      redirect_in = true;
      ifile = i + 1 < argc ? argv[ i + 1 ] : "";

    } else if ( !strcmp( argv[ i ], ">" ) ) {

      if ( redirect_out ) {
        fprintf( stderr, "%s: Illegal output redirect\n", argv[ i + 1 ] );
        exit( EXIT_FAILURE );
      }
      
      redirect_out = true;
      ofile = i + 1 < argc ? argv[ i + 1 ] : "";

    }
  
  }

  /*
  ** Check for valid input redirect
  **/
  FILE *in;
  if ( redirect_in ) {
    in = fopen( ifile, "r" );
    if ( in == NULL ) {
      perror( ifile );
      exit( EXIT_FAILURE );
    }
  }

  /*
  ** Check for valid output redirect
  */
  FILE *out;
  if ( redirect_out ) {
    out = fopen( ofile, "w" );
    if ( out == NULL ) {
      perror( ofile );
      exit( EXIT_FAILURE );
    }
  }

  /*
  ** TODO/Hints:
  **  - Loop to continually process arguments.
  **  - Might need to call dup()/dup2() to allows access to parent descriptors in child.
  **  - Uses  exec/execlp and friends to actuall call process.
  **  - File handeling shit.
  */

//  int cur = 1;
//  while ( true ) {

//    int fd[ 2 ];
//    if ( pipe( fd ) == -1 ) {
      // Errorr!
//    }

//    pid_t childpid = fork();

//    if ( childpid == FORK_ERR ) {
//      perror( "fork" );
//      _exit( EXIT_FAILURE );
//    }


//    if ( childpid == 0 ) {

//      if ( /* we are getting in put on left */ ) {
//        dup2( fds[ IN ], STDIN_FILENO );
//        close( fds[ IN ] );
//      } else if ( /* we are sending output on right */  ) {
//        dup2( fds[ OUT ], STDOUT_FILENO );
//        close( fds[ OUT ] );
//      }

//      if ( redirect_in ) { 
//        rw_to( ifile, stdout );
//        redirect_in = false;
//      }

//      char *const args[] = { sysargv[ cur ] , NULL };
//      execcv( args[ 0 ], args );
//      _exit( EXIT_FAILURE );

//    } else {

//    }

//    close( fd[ IN ] );
//    close( fd[ OUT ] );

//    wait( NULL );
//    wait( NULL );

//  }

  return ( EXIT_SUCCESS );
}
