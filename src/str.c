#include <string.h>
#include <stdlib.h>
#include "str.h"
#include "common.h"
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
    strncat(input, " ",1);
    strncat(input, curr, strlen(curr));
    curr = strtok( NULL, WHITESPACE );
  }

  int len = strlen(input);
  char *output = allocate((len*sizeof(char))+1);

  strncpy(output,input,len+1);
  deallocate(input);
  return output;
}
