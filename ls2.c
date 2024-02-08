#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "ls2.h"


// TODO: function definitions here for ls2
// System calls: opendir(), readdir(), closedir()

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
                    
                    char *input = malloc(sizeof(directory->d_name)+sizeof(char));
                    
                    strcpy(input, filename);
                    strcat(input, directory->d_name);
                    strcat(input, "/");

                    found2 = lsExact(input, searchFile, numTabs+1, stack);
                    free(input);
                    input = NULL;
                    
                    if (found2 == 1){
                        found = 1;
                        found2 = 0;

                        char *result = malloc(sizeof(directory->d_name)+(sizeof(INDENT)*numTabs));
                        
                        strcpy(result, "");
                        for(int i = 0; i < numTabs; i++){
                            strcat(result, INDENT);
                        }  

                        strcat(result, directory->d_name);
                        strcat(result, "/ (directory)");

                        push(stack, result);
                        
                    }
                    
                
                }
            }

            else if(S_ISREG(link.st_mode)){ // if it is a regular file
                if(strcmp(directory->d_name, searchFile) == 0){

                    char* printout = malloc((sizeof(INDENT)*numTabs) + sizeof(directory->d_name) * 2);
                    
                    strcpy(printout, "");
                    for(int i = 0; i < numTabs; i++){
                        strcat(printout, INDENT);
                    }
                    char* size = malloc(sizeof(link.st_size));
                    sprintf(size, "%ld", link.st_size);
                    strcat(printout, directory->d_name);

                    
                    strcat(printout, " (");
                    strcat(printout, size);
                    strcat(printout, " bytes)");

                    push(stack, printout);

                    free(size);
                    size = NULL;
                    found = 1;
                }
            }
        }
    }
    closedir(dir);
    return found;
}



void lsFull(char* filename, int numTabs){
    DIR* dir = opendir(filename);
    struct dirent *directory;
    struct stat link;


    if(dir == NULL){
        printf("%s is not a directory path! Exiting...\n", filename);
        return;
    }
    
    while((directory = readdir(dir)) != NULL){

        char *global_path = malloc(sizeof(char)*4096);
        getcwd(global_path, 4096);
        
        strcat(global_path, "/");
        strcat(global_path, filename);
        strcat(global_path, directory->d_name);
        

        int success = lstat(global_path, &link);
        free(global_path);
        global_path = NULL;
        

        if (success == 0){
            if(S_ISDIR(link.st_mode)){ // if this is a directory
                //printf("%s", directory->d_name);
                if(strcmp(directory->d_name, ".") != 0 && strcmp(directory->d_name, "..") != 0){
                    for(int i = 0; i < numTabs; i++){
                        printf("%s", INDENT);
                    }
                    printf("%s/ (directory)\n", directory -> d_name);
                    char *input = malloc(sizeof(directory->d_name)+sizeof(char));

                    strcpy(input, filename);
                    strcat(input, directory->d_name);
                    strcat(input, "/");
            
                    lsFull(input, numTabs+1);
                    free(input);
                    input = NULL;
                }
            }
            else if(S_ISREG(link.st_mode)){ // if it is a regular file

                for(int i = 0; i < numTabs; i++){
                    printf("%s", INDENT);
                }
                printf("%s (%ld bytes)\n", directory -> d_name, link.st_size);
                
            }
        }
    }
    
}