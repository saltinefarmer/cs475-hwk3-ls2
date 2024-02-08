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

	if (argc <= 1 || argc > 3){
        printf("./ls2 <path> [exact-match-pattern]\n");
        return (0);
    }
	if (argc == 2){
		lsFull(argv[1], 0);
		return (0);
	}
	if (argc == 3){
		stack_t *s = initstack();
		lsExact(argv[1], argv[2], 0, s);

		printstack(s);
		freestack(s);
		return (0);
	}

	// stack stores the lines to print out
	
	return 0;
}
