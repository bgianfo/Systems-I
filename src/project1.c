#include "db.h"
#include "common.h"
#include "project1.h"
#include "allocate.h"

char *search_term;

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
  } else if ( sscanf( command, "artist %s", arg ) == 1 ) {
    search_term = allocate(INPUTLEN*sizeof(char));
    strcpy( search_term, arg );
    return artist_search;
  } else if ( sscanf( command, "title %s", arg ) == 1 ) {
    search_term = allocate(INPUTLEN*sizeof(char));
    strcpy( search_term, arg );
    return title_search;
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
static void stats( dbentry* db, inaction_t action )
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

int main( int argc, char** argv )
{

  if ( argc < 2 ) {
    fputs( DB_FILE_ERROR, stderr );
  } else {
    bool quitloop = false;
    char *input = allocate(INPUTLEN*sizeof(char));

    dbentry* artist_head = NULL;
    dbentry* title_head = NULL;

    char* dbfile = argv[1];
    artist_head = read_db(dbfile);

    if (artist_head == NULL) {
      unallocate(input);
      return EXIT_FAILURE;
    } else {
      sort(&artist_head, &title_head);
    }

    while( !feof(stdin) && !quitloop )
    {
      printf("\n? ");
      fgets( input, INPUTLEN, stdin);
      char* command = trim(input);

      inaction_t action = process(command);
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
          unallocate(search_term);
          break;
        case title_search:
          print_action( title_head, action, search_term );
          unallocate(search_term);
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
          fprintf( stderr, "Unknown command \'%s\'", command );
          break;
      }
      unallocate(command);
    }
    unallocate(input);
    deallocate_db( artist_head );
    printf("\n");
  }
}
