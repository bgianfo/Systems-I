#include <string.h>
#include <stdlib.h>
#include "str.h"
#include "common.h"
#include "allocate.h"

char* trim(char* in)
{
  char input[INPUTLEN];
  input[0] = '\0';

  char* curr;
  curr = strtok( in, WHITESPACE );
  strcat(input, curr);
  curr = strtok( NULL, WHITESPACE );
  while(NULL != curr)
  {
    strcat(input," ");
    strcat(input, curr);
    curr = strtok( NULL, WHITESPACE );
  }

  int length = strlen(input);
  char *output = (char*) allocate(length*sizeof(char));
  strcpy(output,input);

  return output;
}
