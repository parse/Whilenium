#ifndef __SHELL__
#define __SHELL__

#include "Structs.h"
#include "IOHandler.h"
#include "_Boot.h"
#include "stdlib.h"

// User programs
void Shell();
void parseCommand(char* str);
void split(char* cmd, char* args[512]);
char* skipwhite(char* s);
int isspace(char s);

#endif
