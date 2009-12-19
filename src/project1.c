#include "db.h"
#include "common.h"
#include "allocate.h"

char search_term[INPUTLEN];

inaction_t process( char command[] ) 
{
  char arg[INPUTLEN];
  if ( cmps( command, "time" ) ) {
    return time;
  } else if ( cmps( command, "count") ) {
    return count;
  } else if ( cmps( command, "tracks") ) {
    return tracks;
  } else if ( cmps( command, "status") ) {
    return status;
  } else if ( sscanf( command, "artist %s", arg ) == 1 ) {
    strcpy( search_term, arg );
    return artist_search;
  } else if ( sscanf( command, "title %s", arg ) == 1 ) {
    strcpy( search_term, arg );
    return title_search;
  }
  return noop;
}

/**
 * Calculate various statistics about the current database.
 *
 * @param db - Pointer to the head node of the database.
 * @param action - Which actions should 
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
    total_s += current->time_s;
    total_m += current->time_m;
    total_cds++;
    total_tracks += current->tracks;

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
      printf( "%d CDs; %d tracks; total time %d:%d:%d", 
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
    char input[INPUTLEN];

    dbentry* artist_head = NULL;
    dbentry* title_head = NULL;

    char* dbfile = argv[1];
    artist_head = read_db(dbfile);

    sort(&artist_head, &title_head);

    printf("? ");
    while( !feof(stdin) && !quitloop )
    {
      fgets( input, INPUTLEN, stdin);

      inaction_t action = process(input);
      switch( action ) 
      {
        case time: 
          stats( artist_head, action );
          break;
        case count:
          stats( artist_head, action );
          break;
        case tracks:
          stats( artist_head, action );
          break;
        case status:
          stats( artist_head, action );
          break;
        case quit:
          quitloop = true;
          break;
        case noop:

        default:
          printf("No command");
          break;
      }
      printf("\n? ");
    }
  }
}
