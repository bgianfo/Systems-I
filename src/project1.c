#include "db.h"
#include "str.h"
#include "common.h"
#include "project1.h"
#include <allocate.h>

char *search_term;

/**
 * Process the suplied command argument and attempt to resolve it to one of 
 * the actions in the "inaction_t" enum.
 *
 * @param command - The input command to try to process.
 * @return the inacion_t item or the no operation (noop) indicator.
 *
 * @note - possible side effect of setting the search_term pointer on the 
 * title and artist command .
 */
inaction_t process( char* command )
{
  char arg[INPUTLEN];
  if ( cmps( command, "time" ) ) {
    return time;
  } else if ( cmps( command, "quit") ) {
    return quit;
  } else if ( cmps( command, "count") ) {
    return count;
  } else if ( cmps( command, "tracks") ) {
    return tracks;
  } else if ( cmps( command, "status") ) {
    return status;
  } else if ( cmps( command, "artists") ) {
    return artists;
  } else if ( cmps( command, "titles") ) {
    return titles;
  } 
  
  if (sscanf( command, "%s %*s", arg ) == 1) {
    if ( cmps(arg, "artist" ) ) {
      if ( sscanf( command, "%*s %s", arg ) == 1 ) {
        search_term = allocate(INPUTLEN*sizeof(char));
        strcpy( search_term, arg );
        return artist_search;
      }
    } else if ( cmps( arg, "title" ) ) {
      if ( sscanf( command, "%*s %s", arg ) == 1 ) {
        search_term = allocate(INPUTLEN*sizeof(char));
        strcpy( search_term, arg );
        return title_search;
      }
    }
  }
  return noop;
}

/**
 * Calculate various statistics about the current database.
 *
 * @param db - Pointer to the head node of the database.
 * @param action - Which actions should should be
 * completed after collecting stats.
 */
void stats( dbentry* db, inaction_t action )
{
  dbentry* current = db;
  int total_h = 0;
  int total_m = 0;
  int total_s = 0;
  int total_cds = 0;
  int total_tracks = 0;

  /* calculate statistics */
  while ( NULL != current )
  {
    total_s += (int) current->time_s;
    total_m += (int) current->time_m;
    ++total_cds;
    total_tracks += (int) current->tracks;

    current = current->artist_next;
  }

  /* Tally up seconds which could be converted to minutes */
  int tmp = total_s / 60;
  total_s = total_s % 60;

  /* Tally up minutes which could be converted to hours */
  total_m += tmp;
  tmp = total_m / 60;
  total_m = total_m % 60;
  total_h += tmp;

  switch (action)
  {
    case time:
      printf( "total time %d:%d:%d", total_h, total_m, total_s );
      break;
    case count:
      printf( "%d CDs", total_cds );
      break;
    case tracks:
      printf( "%d tracks", total_tracks );
      break;
    case status:
      printf( "\n%d CDs; %d tracks; total time %d:%02d:%02d\n",
          total_cds, total_tracks, total_h, total_m, total_s );
      break;
    default:
      break;
  }
}

inaction_t getinput( char* buffer ) 
{
  printf("\n? ");
  fgets( buffer, INPUTLEN, stdin );
  char* command = trim(buffer);
  inaction_t action = process(command);
  if (action == noop) {
    printf( "\n" );
    fprintf( stderr, "Unknown command \'%s\'\n", command );
  }
  unallocate( command );
  return action;
}

/**
 * Main logic loop 
 *
 * @param argc - number of arguments to the program
 * @param argv - array of arugments to program
 */
int main( int argc, char** argv )
{
  bool quitloop = false;
  dbentry* artist_head = NULL;
  dbentry* title_head = NULL;

  /*
  printf("Size of dbentry* is %ld\n", sizeof(struct dbentry_s*));
  printf("Size of char*    is %ld\n", sizeof(char*));
  printf("Size of uns char is %ld\n", sizeof(unsigned char));
  printf("Size of stucture is %ld\n", sizeof(dbentry));
  */

  if ( argc < 2 ) {
    /** No db file was given, err and quit */
    fputs( DB_FILE_ERROR, stderr );
    return EXIT_FAILURE;
  } else {

   /** The second element will be our db file name/path */
    artist_head = read_db(argv[DB_FILE]);

    if (artist_head == NULL) { /** DB read failed exit */
      return EXIT_FAILURE;
    }

    /* Sort by artist and title */
    sort(&artist_head, &title_head);

    /** DB read failed exit */
    if ( artist_head == NULL && title_head == NULL )  { 
      return EXIT_FAILURE;
    }

    char *buffer = allocate(INPUTLEN*sizeof(char));
    while( !feof(stdin) && !quitloop )
    {
      inaction_t action = getinput( buffer );
      switch( action )
      {
        case artists:
          print_action( artist_head, action, NULL );
          break;
        case titles:
          print_action( title_head, action, NULL );
          break;
        case artist_search:
          print_action( artist_head, action, search_term );
          unallocate( search_term );
          break;
        case title_search:
          print_action( title_head, action, search_term );
          unallocate( search_term );
          break;
        case time:
        case count:
        case tracks:
        case status:
          stats( artist_head, action );
          break;
        case quit:
          quitloop = true;
          break;
        case noop:
        default:
          break;
      }
    }
    unallocate( buffer );
    deallocate_db( artist_head );
    printf("\n");
  }
  return EXIT_SUCCESS;
}
