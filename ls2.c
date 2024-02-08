/*
 * ls2.c
 *
 *  Created on: 2/8/24
 *      Author: Silver Lippert
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "ls2.h"

/**
 * lsExact recurses through the given directory name, and looks for files
 * with the exact specified name. Will create a heap with the files, 
 * and the directories required to traverse through to reach that file
 * 
 * @param char* filename - the file to search through
 * @param char* searchFile - the name of the desired file(s)
 * @param int numTabs - the depth of the recursion to indicate how much tabbing needs to be printed
 * @param stack_t* stack - Stack used to store the files traversed
*/

int lsExact(char* filename, char* searchFile, int numTabs, stack_t* stack){
    
    DIR* dir = opendir(filename);
    struct dirent *directory;
    struct stat link;
    int found = 0;
    
    if(dir == NULL){
        printf("%s is not a directory path! Exiting...\n", filename);
        return (0);
    }

    
    
    while((directory = readdir(dir)) != NULL){
        int found2 = 0; // counter to see what has found the desired file
        char *global_path = malloc(sizeof(char)*4096); // max global path size is apparently 4096
        getcwd(global_path, 4096);
        
        strcat(global_path, "/");
        strcat(global_path, filename);
        strcat(global_path, directory->d_name);
        

        int success = lstat(global_path, &link); // get stats for current file / directory being scanned
        free(global_path); // free global path bc its not needed anymore
        global_path = NULL;
        

        if (success == 0){ // if lstat doesnt encounter any problems

            if(S_ISDIR(link.st_mode)){ // if this is a directory
                
                // if not a parent directory
                if(strcmp(directory->d_name, ".") != 0 && strcmp(directory->d_name, "..") != 0){
                    
                    char *input = malloc(sizeof(directory->d_name)+sizeof(char)); // file name to pass to recursion
                    
                    // I am so sorry if all my string operations are gross, i dont know how else to do it
                    strcpy(input, filename);
                    strcat(input, directory->d_name);
                    strcat(input, "/");

                    found2 = lsExact(input, searchFile, numTabs+1, stack); // recurse
                    free(input);
                    input = NULL;
                    
                    if (found2 == 1){ // if file is found further down
                        found = 1; // set return value to 1 so parent directory knows
                        found2 = 0; // reset in case another directory in this one has another matching file

                        // current directory's name
                        char *result = malloc(sizeof(directory->d_name)+(sizeof(INDENT)*numTabs));
                        
                        strcpy(result, ""); // reset the string in case old data is there
                        for(int i = 0; i < numTabs; i++){ // set number of tabs
                            strcat(result, INDENT);
                        }  

                        strcat(result, directory->d_name);
                        strcat(result, "/ (directory)");

                        push(stack, result); // add to stack!
                        
                    }
                }
            }

            else if(S_ISREG(link.st_mode)){ // if it is a regular file
                if(strcmp(directory->d_name, searchFile) == 0){ // if it matches the desired file name

                    // string to be printed out with file name and other info
                    char* printout = malloc((sizeof(INDENT)*numTabs) + sizeof(directory->d_name) + sizeof(long int) + 9);
                    
                    strcpy(printout, ""); // reset string just in case
                    for(int i = 0; i < numTabs; i++){ // indenting
                        strcat(printout, INDENT);
                    }
                    // another string to contain the byte size
                    char* size = malloc(sizeof(link.st_size));
                    sprintf(size, "%ld", link.st_size);

                    // gross string operations. Lord forgive my soul
                    strcat(printout, directory->d_name);
                    strcat(printout, " (");
                    strcat(printout, size);
                    strcat(printout, " bytes)");

                    push(stack, printout); // add to stack!

                    free(size);
                    size = NULL;
                    found = 1; // matching file was found, return 1 so parent directories know
                }
            }
        }
    }
    closedir(dir); // close the directory
    return found; // return if it was found :)
}

/**
 * lsFull takes in a starting directory, and recursively spits out all of the
 * decendant directories and files.
 * 
 * @param char* filename - the file to search through
 * @param int numTabs - the depth of the recursion to indicate how much tabbing needs to be printed
*/

void lsFull(char* filename, int numTabs){
    DIR* dir = opendir(filename);
    struct dirent *directory;
    struct stat link;


    if(dir == NULL){
        printf("%s is not a directory path! Exiting...\n", filename);
        return;
    }
    
    while((directory = readdir(dir)) != NULL){ // scan through directory

        // create string w/ full directory name
        char *global_path = malloc(sizeof(char)*4096);
        getcwd(global_path, 4096);
        strcat(global_path, "/");
        strcat(global_path, filename);
        strcat(global_path, directory->d_name);
        
        // get data
        int success = lstat(global_path, &link);
        free(global_path);
        global_path = NULL;
        

        if (success == 0){ // if file is successfully loaded
            if(S_ISDIR(link.st_mode)){ // if this is a directory

                // if directory is not ./ or ../
                if(strcmp(directory->d_name, ".") != 0 && strcmp(directory->d_name, "..") != 0){
                    
                    for(int i = 0; i < numTabs; i++){// tabbing
                        printf("%s", INDENT);
                    }
                    // print out directory info
                    printf("%s/ (directory)\n", directory -> d_name);

                    // set up name of file for recursion
                    char *input = malloc(sizeof(directory->d_name)+sizeof(char));
                    strcpy(input, filename);
                    strcat(input, directory->d_name);
                    strcat(input, "/");
            
                    lsFull(input, numTabs+1); // recurse!
                    free(input);
                    input = NULL;
                }
            }

            else if(S_ISREG(link.st_mode)){ // if it is a regular file

                for(int i = 0; i < numTabs; i++){ // tabbing
                    printf("%s", INDENT);
                }
                // print out file info
                printf("%s (%ld bytes)\n", directory -> d_name, link.st_size);
                
            }
        }
    }
    closedir(dir);
}