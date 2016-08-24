/***************************************************************
  Student Name: Mikayla Timm
  File Name: Lexan.c
  Assignment number 2

 * This file implements all of the functions
 * for the lexical analyzer, and symbol table operations. The lexan function was taken from Dr. Coffey's
 * pseudocode. It also sets up the data struct and symbol table.
***************************************************************/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Lexan.h"
/*
 * Set up the struct to hold all of the information used by the whole program in the Data struct.
 * Initialize variables.
 */
DataP createData(FILE *file){
    DataP data = (DataP)malloc(sizeof(struct Data));
    data->table = createTable();
    data->file = file;
    data->lineno = 1;
    data->lookahead = 1;
    data->declsDone = 0;
    data->ch = 0;
    data->currentReg = 0;
    data->comma = ",";
    return data;
}
/*
 * Create the symbol table and set up the 1st and 2nd elements for begin and end.
 * Return the table to the data struct.
 */
SymbolTableP createTable(){
    SymbolTableP table = (SymbolTableP)malloc(sizeof(struct SymbolTable));
    table->size = 3; //start with 3: begin and end at pos 1 and 2, 0 empty
    table->symbolPs[0] = NULL;
    table->symbolPs[1] = (SymbolP)malloc(sizeof(struct Symbol));
    table->symbolPs[1]->type = BEGIN;
    strcpy(table->symbolPs[1]->value,"begin");
    table->symbolPs[2] = (SymbolP)malloc(sizeof(struct Symbol));
    table->symbolPs[2]->type = END;
    strcpy(table->symbolPs[2]->value, "end");
    return table;
}
/*
 * Finds an identifier value in the symbol table held in the data struct.
 * If not found, return NOT_FOUND value. Else return the index where the ID was found.
 */
int find(char *value, DataP data){
    int i;
    for(i = 1; i < data->table->size; i++){
        //go through table until value is found. return index
        if(strcmp(data->table->symbolPs[i]->value, value ) == 0){
            return i; //found the value in the table
        }
    }
    //couldn't find value in table.
    return NOT_FOUND;
}
/*
 * Inserts an identifier into the symbol table held in the data struct.
 */
void insert(char *value, DataP data){
    SymbolP newSymbol = (SymbolP)malloc(sizeof(struct Symbol));
    strcpy(newSymbol->value, value);
    newSymbol->type = ID;
    data->table->symbolPs[data->table->size] = newSymbol;
    (data->table->size)++;
}
/*
 * Lexan function that processes all of the characters
    data->assignVar = (char *)in the program, deciding if
 * it's an ID, NUM, comment, new line or space or tab, or operator. Returns the character to the parser.
 * Taken from Dr. Coffey's pseudocode. Please don't count off for being too long...I did my best to factor it
 */
int lexan(DataP data){
    while(1){
        data->ch = fgetc(data->file);
        if(data->ch == ' ' || data->ch == '\t'){
            ;
        }
        else if(data->ch == '~'){
            //found comment, continue reading/throwing out chars until new line reached
            while(data->ch != '\n' && data->ch != '\r' && data->ch != EOF) {
                data->ch = fgetc(data->file);
            }
            if(data->ch == EOF){
                return DONE;
            }
            data->lineno++;
        }
        else if(data->ch == '\n'){
            data->lineno++;
        }
        else if(isdigit(data->ch)){
            readNum(data);
            return NUM;
        }
        else if(isalpha(data->ch)){
            readID(data); //read the rest of the ID in (legal or illegal)
            int legal = checkIfIDLegal(data); //check if ID is legal after reading in.
            if(legal == 1) {
                if (strcmp(data->identifier, "int") == 0) {
                    getInts(data);
                }
                else {
                    data->pos = find(data->identifier, data);
                    if (data->pos == NOT_FOUND) {
                        //undeclared variable being referenced. error
                        fprintf(stderr, "Line %d contains error: Undefined variable %s.\n", data->lineno,
                                data->identifier);
                        stopProgram(data);
                    }
                    else if (data->pos == 1) {
                        return BEGIN;
                    }
                    else if (data->pos == 2) {
                        return END;
                    }
                    else {
                        //found an ID that is in the table. set declsdone to 1 from here on out
                        data->declsDone = 1;
                        return ID;
                    }
                }
            }
        }
        else if(data->ch == EOF){
            return DONE;
        }
        else{
            return data->ch; //return the character to parser
        }
    }
}
/*
 * Reads in all of the characters for each ID (alphas, digits, and _s).
 */
void readID(DataP data){
    //get the identifier into value. read in until you reach something
    //that's not a letter, digit, or underscore
    char c[2];
    data->identifier[0] = (char)data->ch;
    data->identifier[1] = '\0';
    //put first char into identifier with null terminator. read next chars
    data->ch = fgetc(data->file);
    while(isalpha(data->ch) || isdigit(data->ch) || data->ch == '_'){
        c[0] = (char)data->ch;
        c[1] = '\0';
        strcat(data->identifier, c);
        data->ch = fgetc(data->file);
    }
    //reached the end of the identifier. unget the char that wasn't digit, alpha, or _

    ungetc(data->ch, data->file);
}
/*
 * Reads in the number and saves it into identifier
 */
void readNum(DataP data){
    char c[2];
    data->identifier[0] = (char)data->ch;
    data->identifier[1] = '\0';
    //put first char into identifier with null terminator. read next chars
    data->ch = fgetc(data->file);
    while(isdigit(data->ch)){
        c[0] = (char)data->ch;
        c[1] = '\0';
        strcat(data->identifier, c);
        data->ch = fgetc(data->file);
    }
    //found a nondigit. unget
    ungetc(data->ch, data->file);
}
/*
 * Checks the ID just read in to see if it's legal, as in no consecutive _s and
 * not ending in an _.
 */
int checkIfIDLegal(DataP data){
    int i = 0;
    int legal = 1;
    while(data->identifier[i] != '\0'){
        if(data->identifier[i] == '_'){
            if(data->identifier[i+1] == '_'){
                //two underscores in a row, illegal
                fprintf(stderr, "Line %d contains error: illegal identifier - more than 1 _ in a row.\n", data->lineno);
                stopProgram(data);
            }
        }
        i++;
    }
    if(data->identifier[i-1] == '_'){
        //ended with an underscore. illegal
        fprintf(stderr, "Line %d contains error: illegal identifier - ended in _.\n", data->lineno);
        stopProgram(data);
    }
    return legal;
}

/*
 * Reads in the int IDs until a semicolon is reached.
 */
void getInts(DataP data){
    if(data->declsDone == 1){//declarations after assignment statements. Illegal.
        fprintf(stderr, "Line %d contains error: declaration found after assignment statements.\n", data->lineno);
        stopProgram(data);
    }
    while(data->ch != ';' && data->ch != EOF) {
        getToID(data);
        readID(data);
        if (strcmp(data->identifier, "int" ) == 0){ //tried to use int as an ID
            fprintf(stderr, "Line %d contains error: cannot use reserved word as identifier.\n", data->lineno);
            stopProgram(data);
        }
        int legal = checkIfIDLegal(data);
        if(legal == 0){
            fprintf(stderr, "Line %d contains error: illegal ID '%s'.\n", data->lineno, data->identifier);
            stopProgram(data);
        }
        if(find(data->identifier, data) == NOT_FOUND && legal == 1){
            insert(data->identifier, data); //insert value into symbol table
        }
        else{//illegal redef
            fprintf(stderr, "Line %d contains error: illegal redefinition of %s.\n", data->lineno, data->identifier);
            stopProgram(data);
        }
        getToEnd(data);
        if (data->ch == ',')
        {
            getToID(data);
        }
        else{
            //reached a semicolon. done
            return;
        }

    }
}

/*
 * Reads until a , or ; is reached
 */
void getToEnd(DataP data){
    data->ch = fgetc(data->file); //should be either a comma, newline, tab/space, ~, or semicolon. if not error
    while(data->ch != ',' && data->ch != ';') { //get to the comma or semicolon so you can either get next ID or kick out
        if(data->ch != ' ' && data->ch != '\t' && data->ch != '\n' && data->ch != '~') { //not a , \n, tab, space, ~, or ; so error
            fprintf(stderr, "Line %d contains error: unexpected token - '%c'.\n", data->lineno, data->ch);
            stopProgram(data);
        }
        else if (data->ch == '~') {
            //found comment, continue reading/throwing out chars until new line reached
            while (data->ch != '\n' && data->ch != '\r' && data->ch != EOF) {
                data->ch = fgetc(data->file);
            }
            if (data->ch == EOF) {
                fprintf(stderr, "Line %d contains error: no semicolon.\n", data->lineno);
                stopProgram(data);
            }
            data->lineno++;
        }
        else if(data->ch == '\n'){
            data->lineno++;
        }
        data->ch = fgetc(data->file);
    }
}
/*
 * Reads until next ID
 */
void getToID(DataP data){
    //currently data->ch is a ,  or ' ' and we want to get to a letter. keep reading till we get to a valid letter
    if(data->ch == ',') {
        data->ch = fgetc(data->file);
    }
    while(!isalpha(data->ch)){
        if(data->ch != ' ' && data->ch != '\t' && data->ch != '\n' && data->ch != '~') { //not a , \n, tab, space, ~, or ; so error
            fprintf(stderr, "Line %d contains error: unexpected token - '%c'.\n", data->lineno, data->ch);
            stopProgram(data);
        }
        else if (data->ch == '~') {
            //found comment, continue reading/throwing out chars until new line reached
            while (data->ch != '\n' && data->ch != '\r' && data->ch != EOF) {
                data->ch = fgetc(data->file);
            }
            if (data->ch == EOF) {
                fprintf(stderr, "Line %d contains error: no semicolon.\n", data->lineno);
                stopProgram(data);
            }
            data->lineno++;
        }
        else if(data->ch == '\n'){
            data->lineno++;
        }
        data->ch = fgetc(data->file);
    } //should end with data->ch being the first letter of the ID.
}
/*
 * exits the program. closes the files
 */
void stopProgram(DataP data){
    fclose(data->file);
    fclose(data->outFile);
    exit(1);
}
