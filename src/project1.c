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
