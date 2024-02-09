#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "ls2.h"

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char* argv[]) {

	if (argc <= 1 || argc > 3){ // if theres an incorrect number of arguments
        printf("./ls2 <path> [exact-match-pattern]\n");
        return (0);
    }
	if (argc == 2){ // if only the directory to be searched is provided
		lsFull(argv[1], 0);
		return (0);
	}
	if (argc == 3){ // if directory and file to be searched for is provided
		stack_t *s = initstack();
		lsExact(argv[1], argv[2], 0, s);

		printstack(s);
		freestack(s);
		return (0);
	}
	
	return 0;
}
