#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#include <errno.h>
#include <assert.h>

#include "common.h"

/* XXX: CHANGE ME */
/*#include <allocate.h>*/

inaction_t process( char command[] ) 
{
  if ( strcmp( command, "time" ) ) {
    return time;
  } else if ( strcmp( command, "count" ) ) {
    return count;
  } else if ( strcmp( command, "tracks" ) ) {
    return tracks;
  } else if ( strcmp( command, "status" ) ) {
    return status;
  }

  return none;
}


int main( void ) 
{
  char input[INPUT_LEN];

  printf("? ");
  while( !feof(stdin) )
  {
    fgets(input, INPUT_LEN, stdin);

    inaction_t action = process(input);
    switch( action ) 
    {
      /* Fall through on these cases */
      case time: 
      case count:
      case tracks:
      case status:
        stats( action );
        break;
      default:
        printf("No command\n");
        break;
    }
    printf("\n? ");
  }

  if (feof(stdin)) {
    printf("eof detected!"); 
  }
}
