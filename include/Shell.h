#ifndef __SHELL__
#define __SHELL__

#include "Structs.h"
#include "IOHandler.h"
#include "_Boot.h"
#include "stdlib.h"

// User programs
void Shell();
char* split(char* cmd);
char* skipwhite(char* s);
int isspace(char s);

#endif
