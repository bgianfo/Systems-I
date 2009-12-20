#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "str.h"
#include "allocate.h"

char* trim(char* in)
{
  char* input = allocate(INPUTLEN*sizeof(char));
  input[0] = '\0';

  char* curr = strtok( in, WHITESPACE );

  strncat(input, curr, strlen(curr));

  curr = strtok( NULL, WHITESPACE );

  while(NULL != curr)
  {
    strncat(input, " ", 1);
    strncat(input, curr, strlen(curr));
    curr = strtok( NULL, WHITESPACE );
  }

  int len = strlen(input);
  char *output = allocate((len*sizeof(char))+1);

  strncpy(output,input,len+1);
  unallocate(input);

  return output;
}

char* trim_limit( char* in, inaction_t action )
{
  char *stripped = trim(in);
  if (action == artists) {
    if ( strlen( stripped ) > 25 ) {
      char* limited = allocate((25*sizeof(char))+1);
      strncpy(limited, stripped, 25);
      limited[25] = '\0';
      unallocate(stripped);
      return limited;
    }
  } else if (action == titles) {
    if ( strlen( stripped ) > 40 ) {
      char* limited = allocate((40*sizeof(char))+1);
      strncpy(limited, stripped, 40);
      limited[40] = '\0';
      unallocate(stripped);
      return limited;
    }
  }
  return stripped;
}
