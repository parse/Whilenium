#ifndef __STDLIB__
#define __STDLIB__

#include "Structs.h"

typedef unsigned int size_t;

char strcmp(char* strA, char* strB);
char *itoa(int number, char *buffer, int radix);
size_t strlen(const char *str);
char *strrev(char *str);
char *strcpy(char *target, const char *source);
char *strchr(const char *s, int ch);
int atoi(const char *s);

#endif
