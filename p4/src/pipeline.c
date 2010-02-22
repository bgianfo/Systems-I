/* ** File:  pipeline.c
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

/* redirect input/output */
bool redirect_out = false;

/* filename for input/output files */
char* ifile;
char* ofile;


void do_child( char** argv, char** args, int argc, bool last_cmd );

void debug( char str[], char** argv ) {

    //fprintf( stderr, " %s argc: %d\n", str, *argc );
    fprintf( stderr, "%s { ", str );
    int iter = 0;
    while ( argv[iter] ) {
      fprintf(stderr, "\"%s\", ",argv[iter]);
      iter++;
    }
    fprintf(stderr, "NULL }\n");
    return;
}

bool next_cmd( char*** argv, char*** args, int *argc ) {

  if ( (*argc) <= 0 ) {
    return false;
  }
  //debug("before: ", *args );
  (*args) = (*argv);
  for ( ; (*argc) >= 0; (*argc)-- ) {
    if ( (*argv)[ 0 ] == NULL ) {
      (*argv)--;
      (*argc)--;
      (*args) = (*argv) + 2;
      break;
    } else {
      (*argv)--;
    }

  }

  int i = 0;
  while ( (*args)[ i ] != NULL ) {
    if ( (*args)[ i ][ 0 ] == '<' || (*args)[ i ][ 0 ] == '>' ) {
      (*args)[ i ] = (char*) NULL;
      break;
    }
    i++;
  }

  return true;
}

int main( int argc, char* argv[] ) {

  /* Just exit if no arguments */
  if ( argc < 2 ) {
    exit( EXIT_SUCCESS );
  }

  /*
  ** Check if the first or last command are pipes;
  ** if so print out an error message and exit
  */
  if ( argv[ 1 ][ 0 ] == '|' || argv[ argc - 1 ][ 0 ] == '|' ) {
    fprintf( stderr, "Illegal null command\n" );
    exit( EXIT_FAILURE );
  }

  /* Check for missing command name */
  if ( argv[ 1 ][ 0 ] == '>' || argv[ 1 ][ 0 ] == '<' ) {
    fprintf( stderr, "Missing command name\n" );
    exit( EXIT_FAILURE );
  }
  /*
  ** Loop through arguments and check for illegal
  ** null commands before/after pipe, illegal input
  ** redirects and illegal output redirects
  */
  bool block_in = false;
  bool redirect_in = false;
  for ( int i = 2; i < argc - 1; i++ ) {

    if ( argv[ i ][ 0 ] == '|' ) {

      if ( argv[ i - 1 ][ 0 ] == '|' || argv[ i + 1 ][ 0 ] == '|' ) {
        fprintf( stderr, "Illegal null command\n" );
        exit( EXIT_FAILURE );
      }

      if ( redirect_out ) {
        fprintf( stderr, "%s: Illegal output redirect\n", argv[ i - 1 ] );
        exit( EXIT_FAILURE );
      }

      block_in = true;

    } else if ( argv[ i ][ 0 ] == '<' ) {

      if ( block_in || redirect_in ) {
        fprintf( stderr, "%s: Illegal input redirect\n", argv[ i + 1 ] );
        exit( EXIT_FAILURE );
      }

      redirect_in = true;
      ifile = i + 1 < argc ? argv[ i + 1 ] : "";

    } else if ( argv[ i ][ 0 ] == '>' ) {

      if ( redirect_out ) {
        fprintf( stderr, "%s: Illegal output redirect\n", argv[ i + 1 ] );
        exit( EXIT_FAILURE );
      }

      redirect_out = true;
      ofile = i + 1 < argc ? argv[ i + 1 ] : "";
    }
  }

  /* Check for valid input redirect */
  if ( redirect_in ) {
    if ( freopen( ifile, "r", stdin ) == NULL ) {
      perror( ifile );
      exit( EXIT_FAILURE );
    }
  }

  /* Check for valid output redirect */
  if ( redirect_out ) {
    FILE *out;
    if ( ( out = fopen( ofile, "w" ) ) == NULL ) {
      perror( ofile );
      exit( EXIT_FAILURE );
    }
    fclose( out );
  }

  /*
  ** TODO/Hints:
  **  - Loop to continually process arguments.
  **  - Might need to call dup()/dup2() to allows access to parent descriptors in child.
  **  - Uses  exec/execlp and friends to actuall call process.
  **  - File handeling shit.
  */

  argv[ 0 ] = ( char* ) NULL;
  for ( int i = 1; i < argc; i++ ) {
    if ( argv[ i ][ 0 ] == '|' ) {
      argv[ i ] = ( char* ) NULL;
    }
  }

  argc--;
  argv += argc;
  char** args = argv;

  /* Move to last argument */
  next_cmd( &argv, &args, &argc );

  /* Start off the recursive forking fun */
  do_child( argv, args, argc, true );

  return ( EXIT_SUCCESS );
}

void do_child( char** argv, char** args, int argc, bool last_cmd ){

  int status;
  int fds[2] = { -1, -1 };
  status = pipe( fds );

  if ( status == -1) {
    perror( "failed pipe" );
    fprintf( stderr, "pipe failed: [0] = %d, [1] = %d\n", fds[0], fds[1] );
  }

  pid_t pid = fork();

  /* Child  now */
  if ( pid == 0 ) {
    char** cmd = args;

    /*
    if ( argc <= 0 ) {
      fprintf( stderr, "first command: %s\n", cmd[ 0 ] );
      dup2( fds[ 0 ], STDIN_FILENO );
    }
    */
    //dup2( fds[ 0 ], STDIN_FILENO );
    
    if ( last_cmd ) {
      dup2( 1, STDOUT_FILENO );
      freopen( ofile, "w", stdout );
    } else {
      dup2( fds[ 1 ], STDOUT_FILENO );
    }
    close( fds[ 0 ] );

    if ( next_cmd( &argv, &args, &argc ) ) {
      do_child( argv, args, argc, false );
    }

    //fprintf( stderr, "command: %s\n", cmd[0] );
    execvp( cmd[ 0 ], cmd );
  } else {
    //fprintf( stderr, "waiting parent of %s \n", args[0] );

    dup2( fds[ 0 ], STDIN_FILENO );
    close( fds[ 1 ] );
    wait( NULL );
 
    /*
    if ( last_cmd ) {
      fprintf( stderr, "last command: %s\n", args[ 0 ] );
      dup2( fds[ 1 ], STDOUT_FILENO );
    } 
    close( fds[ 1 ] );
    wait( NULL );
    */

    //fprintf( stderr, "end waiting parent %s \n", args[0] );
  }
  return;
}
