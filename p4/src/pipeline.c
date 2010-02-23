/* File:  pipeline.c
**
** Authors:
**    Brian Gianforcaro (bjg1955@cs.rit.edu)
**    Nicholas Williams (nxw6572@cs.rit.edu)
**
** Description:
**    Implementing pipes and i/o redirection, this program performs
**    a command sequence by creating child processes connected with
**    pipes which execute the individual commands.
**
**    ex: pipeline command args "<" file "|" command args ">" file
**
*/

#include "pipeline.h"

/* Redirect input/output */
bool redirect_out = false;

/* Filename for input/output files */
char* ofile;

/*
** Sets up the next command (args) for execvp in child process
** NOTE: Processing commands from back to front.
*/
bool next_cmd( char*** argv, char*** args, int *argc ) {

  if ( ( *argc ) <= 0 ) {
    return false;
  }

  ( *args ) = ( *argv );
  for ( ; ( *argc ) >= 0; ( *argc )-- ) {
    if ( ( *argv )[ 0 ] == NULL ) {
      ( *argv )--;
      ( *argc )--;
      ( *args ) = ( *argv ) + 2;
      break;
    } else {
      ( *argv ) --;
    }

  }

  int i = 0;
  while ( ( *args )[ i ] != NULL ) {
    if ( ( *args )[ i ][ 0 ] == '<' || ( *args )[ i ][ 0 ] == '>' ) {
      ( *args )[ i ] = ( char* ) NULL;
      break;
    }
    i++;
  }

  return true;
}

/*
** Recursive method for setting up fork() with multiple commands.
*/
void do_child( char** argv, char** args, int argc, bool last_cmd ) {

  int fds[ 2 ];

  if ( pipe( fds ) == ERROR ) {
    perror( "failed pipe" );
    _exit( EXIT_FAILURE );
  }

  pid_t pid = fork();

  if ( pid == ERROR ) {
    perror( "failed fork" );
    _exit( EXIT_FAILURE );
  }

  if ( pid == 0 ) {
    
    char** cmd = args;

    if ( last_cmd ) {

      dup3( 1, STDOUT_FILENO );

      if ( redirect_out ) {
        if ( freopen( ofile, "w", stdout ) == NULL ) {
          perror( ofile );
          _exit( EXIT_FAILURE );
        }
      }
    } else {
      dup3( fds[ 1 ], STDOUT_FILENO );
    }
    
    close2( fds[ 0 ] );

    if ( next_cmd( &argv, &args, &argc ) ) {
      /* Recursive call to start processing next argument */ 
      do_child( argv, args, argc, false );
    }

    execvp( cmd[ 0 ], cmd );

  } else {

    dup3( fds[ 0 ], STDIN_FILENO );
    close2( fds[ 1 ] );
    wait( NULL );
 
  }

  return;
}

/* The main program */
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

      char* ifile = i + 1 < argc ? argv[ i + 1 ] : "";

      /* Check for valid input file and redirect to stdin */
      if ( freopen( ifile, "r", stdin ) == NULL ) {
        perror( ifile );
        exit( EXIT_FAILURE );
      }
      redirect_in = true;

    } else if ( argv[ i ][ 0 ] == '>' ) {

      if ( redirect_out ) {
        fprintf( stderr, "%s: Illegal output redirect\n", argv[ i + 1 ] );
        exit( EXIT_FAILURE );
      }

      ofile = i + 1 < argc ? argv[ i + 1 ] : "";

      /* Check for valid output file */
      FILE *out;
      if ( ( out = fopen( ofile, "w" ) ) == NULL ) {
        perror( ofile );
        exit( EXIT_FAILURE );
      }
      if ( ERROR == fclose( out ) ) {
        perror( "fclose" );
        exit( EXIT_FAILURE );
      }
      redirect_out = true;
    }
  }

  /* Replace the first argument (process name) with a NULL */
  argv[ 0 ] = ( char* ) NULL;

  /* Replace pipes with a NULL for processing with execvp */
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

  /* Start recursive calls for forking command processes */
  do_child( argv, args, argc, true );

  /* Close any actual files still hanging out */
  return ( EXIT_SUCCESS );
}
