/***************************************************************
  Student Name: Mikayla Timm
  File Name: Parser.c
  Assignment number 2

 * This file implements all of the functions
 * for the parser. Most of this was taken from Dr. Coffey's pseudocode.
 * Exits the program if there is an error.
***************************************************************/

#include <stdlib.h>
#include <string.h>
#include "Parser.h"

/*
 * Taken from Dr. Coffey's pseudocode.
 * Checks lexan for an ID, calls the expression function to get an assignment statement from the program.
 */
void AssignStmt(DataP data){
    match(ID, data);
    //copy the assignment variable ID so we can show what it's equal to later
    strcpy(data->assignVar, data->identifier);
    strcpy(data->postfixExpr, "\0"); //reset expression
    if(data->lookahead != '='){
        //error
        fprintf(stderr, "Line %d contains error: expected assignment operator.\n", data->lineno);
        stopProgram(data);
    }
    else{
        match(data->lookahead, data);
        expression(data);
        match(';', data);
        fprintf(data->outFile,"%s = R%d\n", data->assignVar, --data->currentReg);
        removeFinalComma(data);
        fprintf(data->outFile,"*****[%s]*****\n", data->postfixExpr);
        data->currentReg = 0;
    }
}
/*
 * evaluates expressions by checking term, operator, term. Can be recursively called for nested expressions in factor.
 */
void expression(DataP data){
    term(data);
    while(data->lookahead == '+' || data->lookahead == '-'){
        char *operator;
        if(data->lookahead == '+')
            operator =  "+";
        else
            operator = "-";
        match(data->lookahead, data);
        term(data);

        data->currentReg-=2;
        int temp = data->currentReg + 1;
        fprintf(data->outFile,"R%d = R%d %s R%d\n", data->currentReg, data->currentReg, operator, temp);
        data->currentReg++;
        strcat(data->postfixExpr, operator);
        strcat(data->postfixExpr, data->comma);
    }
}
/*
 * Evaluates a "term" by calling factor and making sure the term is made up of either
 * just an ID or NUM, OR an ID/NUM * or / an ID/NUM, or a nested term with parens.
 */
void term(DataP data){
    factor(data);
    while(data->lookahead == '*' || data->lookahead == '/'){
        char *operator;
        if(data->lookahead == '*')
            operator =  "*";
        else
            operator = "/";
        match(data->lookahead, data);
        factor(data);
        data->currentReg-=2;
        int temp = data->currentReg + 1;
        fprintf(data->outFile,"R%d = R%d %s R%d\n", data->currentReg, data->currentReg, operator, temp);
        data->currentReg++;
        strcat(data->postfixExpr, operator);
        strcat(data->postfixExpr, data->comma);
    }
}
/*
 * "Factors" the statement by either just confirming there is an ID or NUM where expected, or
 * if there is a nested expression () recursively calls expression and matches closing paren.
 */
void factor(DataP data){
    if(data->lookahead == ID){
        match(ID, data);
        strcat(data->postfixExpr, data->identifier);
        strcat(data->postfixExpr, data->comma);
        fprintf(data->outFile,"R%d = %s\n", data->currentReg++, data->identifier);
    }
    else if(data->lookahead == NUM){
        match(NUM, data);
        strcat(data->postfixExpr, data->identifier);
        strcat(data->postfixExpr, data->comma);
        fprintf(data->outFile,"R%d = %s\n", data->currentReg++, data->identifier);
    }
    else if(data->lookahead == '('){
        match('(', data);
        expression(data);
        match(')', data);
    }
    else{
        fprintf(stderr, "Line %d contains error: expected '('.\n", data->lineno);
        stopProgram(data);
    }
}
/*
 * Calls lexan to see if character/ID/NUM read in matches "t" passed in. If not, prints an error and closes program.
 */
void match(int t,DataP data){
    if(data->lookahead == t){
        data->lookahead = lexan(data);
    }
    else{
        if(t == BEGIN){
            //no begin to start program. exit
            fprintf(stderr, "Line %d contains error: Reached tokens without expected begin statement.\n", data->lineno);
            stopProgram(data);
        }
        else if(t == ID){
            //expected ID, did not find.
            fprintf(stderr, "Line %d contains error: Expected ID.\n", data->lineno);
            stopProgram(data);
        }
        fprintf(stderr, "Line %d contains error: syntax error - expected '%c'.\n", data->lineno, (char)t);
        stopProgram(data);
    }
}
/*
 * Parses the statements in the program. Reads through until end of file is reached, making calls to other functions
 * to check if all statements are legal. If so, it lists all the IDs in the symbol table. If not, exits with error message.
 */
void parse(DataP data){
    data->lookahead = lexan(data);
    match(BEGIN, data);
    while(data->lookahead != END){
        AssignStmt(data);
    }
    match(END, data);
    //while end of file is not reached, keep checking if there is anything else
    while(data->lookahead != DONE){
        data->lookahead = lexan(data);
        if(data->lookahead != ' ' || data->lookahead != '\n' || data->lookahead != '\t' || data->lookahead != '\r'){
            //something other than white space or comments found.
            fprintf(stderr, "Line %d contains error: text found after end statement. Exiting.\n", data->lineno-1);
            stopProgram(data);
        }
    }
    printf("The program is legal. Output is in file %s\n", data->fileName);
    fclose(data->file);
}

void removeFinalComma(DataP data){
    int i = 0;
    char c = data->postfixExpr[i];
    while(c != '\0'){
        i++;
        c = data->postfixExpr[i];
    }
    data->postfixExpr[i-1] = '\0';
}