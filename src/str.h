#ifndef __STR_H__
#define __STR_H__

#include "common.h"

#define WHITESPACE " \n"

char* trim(char* in);

char* trim_limit(char* in, inaction_t action);

#endif
