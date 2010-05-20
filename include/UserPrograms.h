#ifndef __UPROGRAMS__
#define __UPROGRAMS__

#include "API.h"
#include "Shell.h"

// User programs
void HelloWorld();
void Scroller(char* msg);

void Increment(int a);
void _increment(int n);

void Fibonacci(int a);
int _fibonacci(int n_1, int n_2, int i);

void initUserPrograms();
void ASCII();
#endif
