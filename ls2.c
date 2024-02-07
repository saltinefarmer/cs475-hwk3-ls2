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

void test(char* filename, int numTabs){
    DIR* dir = opendir(filename);
    struct dirent *directory;
    struct stat link;

    //printf("%s\n", filename);
    
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
            
                    test(input, numTabs+1);
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