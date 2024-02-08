#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "stack.h"

#define TRUE 1
#define FALSE 0
#define INDENT "    "

// TODO: Function declarations here for ls2

void lsFull(char* filename, int numTabs);

int lsExact(char* filename, char* searchFile, int numTabs, stack_t* stack);

#endif
