/***************************************************************
  Student Name: Mikayla Timm
  File Name: RecursiveDescent.c
  Assignment number 2

 * This file opens the file for reading and calls the parser to analyze the program.
***************************************************************/

#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Get the filename of the file passed in without the extension
 */
void getFileName(DataP data, char *arg);
/*
 * Opens the file for reading and calls the parser to analyze the program.
 */
int main(int argc, char *argv[]){
    //open the file and set up all the data needed for analyzing and parsing
    DataP data = createData(fopen(argv[1], "r"));
    if(data->file == NULL){
        fprintf(stderr, "null file pointer. Exiting.\n");
        exit(1);
    }
    printf("Compiling %s\n", argv[1]);
    getFileName(data, argv[1]);
    //parse.
    parse(data);
    fclose(data->outFile);
    return 0;
}
/*
 * Get the filename of the file passed in without the extension
 */
void getFileName(DataP data, char *arg){
    data->fileName[0] = '\0';
    int counter = 0;
    char character[2];
    character[0] = arg[counter];
    character[1] = '\0';
    while(arg[counter] != '.'){
        strcat(data->fileName, character);
        counter++;
        character[0] = arg[counter];
    }
    strcat(data->fileName, ".out");
    data->outFile = fopen(data->fileName, "w");
}
