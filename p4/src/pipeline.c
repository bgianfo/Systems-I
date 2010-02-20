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

  /* redirect input/output */
  bool redirect_in = false;
  bool redirect_out = false;

  /* filename for input/output files */
  char* ifile;
  char* ofile;

  /*
  ** Loop through arguments and check for illegal
  ** null commands before/after pipe, illegal input
  ** redirects and illegal output redirects
  */
  bool block_in = false;
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

  for (int i = 0; i < argc; i++ ) {
    if ( argv[ i ][ 0 ] == '>' || argv[ i ][ 0 ] == '<' ) {
      argv[ i ] = ( char* ) NULL;
      // TODO: Might need argc -= (i+1) ?
      argc -= i;
      break;
    }
  }
  argv++;
  argc--;

  /*
  ** TODO/Hints:
  **  - Loop to continually process arguments.
  **  - Might need to call dup()/dup2() to allows access to parent descriptors in child.
  **  - Uses  exec/execlp and friends to actuall call process.
  **  - File handeling shit.
  */

  pid_t childpid;
  int fd_in [ 2 ];
  int fd_out[ 2 ];

  while ( true ) {

    if ( pipe( fd_in ) == ERROR || pipe( fd_out ) == ERROR ) {
      perror( "failed pipe" );
      _exit( EXIT_FAILURE );
    }

    bool argv_mod = false;
    for (int i = 0; i < argc; i++ ) {
      if (  argv[ i ] == NULL ) {
        argv += i;
        // TODO: Might need argc -= (i+1) ?
        argc -= i;
        argv_mod = true;
        break;
      }
    }

    if ( !argv_mod ) {
      argv++;
      argc--;
    }

    if ( argc > 2 ) {
      for (int i = 0; i < argc; i++ ) {
        if ( argv[ i ][ 0 ] == '|' ) {
          argv[ i ] = ( char* ) NULL;
          break;
        }
      }
    } else {
      argv++;
      argc--;
    }

    if ( ( childpid = fork() ) == ERROR ) {
      perror( "failed fork" );
      _exit( EXIT_FAILURE );
    }


    if ( childpid == 0 ) {

      if ( close( fd_in[ 0 ] ) == ERROR || close( fd_out[ 1 ] ) == ERROR ) {
        close( fd_in [ 1 ] );
        close( fd_out[ 0 ] );
        close( fd_out[ 1 ] );
        perror( "failed close" );
        _exit( EXIT_FAILURE );
      }

      if ( dup2( fd_out[ 0 ], STDIN_FILENO ) == ERROR ) {
        close( fd_in [ 1 ] );
        close( fd_out[ 0 ] );
        perror( "failed redirect in" );
        _exit( EXIT_FAILURE );
      } else if ( close( fd_out[ 0 ] ) == ERROR ) {
        close( fd_in [ 1 ] );
        perror( "failed close" );
        _exit( EXIT_FAILURE );
      }

      if ( dup2( fd_in[ 1 ], STDOUT_FILENO ) == ERROR ) {
        close( fd_in[ 1 ] );
        perror( "failed redirect out" );
        _exit( EXIT_FAILURE );
      } else if ( close( fd_in[ 1 ] ) == ERROR ) {
        perror( "failed close" );
        _exit( EXIT_FAILURE );
      }

      if ( redirect_in ) {
        fclose( stdin );
        redirect_in = false;
      }

      fprintf(stderr, "command: %s\n",argv[0] );
      if ( execvp( argv[ 0 ], (argv+1) ) != 0 ) {
        perror( "Error in execvp" );
        _exit( EXIT_FAILURE );
      }

    } else {

      if ( close( fd_in[ 1 ] ) == ERROR || close( fd_out[ 0 ] ) == ERROR ) {
        close( fd_out[ 0 ] );
        perror( "failed close" );
        exit( EXIT_FAILURE );
      } else {
        wait( NULL );
      }
    }
  }

  return ( EXIT_SUCCESS );
}
