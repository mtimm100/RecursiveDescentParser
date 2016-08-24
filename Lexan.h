/***************************************************************
  Student Name: Mikayla Timm
  File Name: Lexan.h
  Assignment number 2

 * This file outlines the functions used by the lexical analyzer and
 * defines the struct used to hold all of the Data necessary to perform the operations,
 * including the file, lookahead, current char (ch), Symbol table, and symbols.
***************************************************************/
#ifndef RECURSIVEDESCENT_LEXAN_H
#define RECURSIVEDESCENT_LEXAN_H
#include <stdio.h>

#define MAXARRAYSIZE 1000
#define ID 300
#define NUM 301
#define BEGIN 400
#define END 401
#define DONE 500
#define NOT_FOUND 501

typedef struct Symbol *SymbolP;
typedef struct SymbolTable *SymbolTableP;
typedef struct Data *DataP;

//Symbol struct
struct Symbol{
    char value[MAXARRAYSIZE];
    int type;
    int pos;
};

//SymbolTable struct
struct SymbolTable{
    SymbolP symbolPs[MAXARRAYSIZE]; //array of symbolPs
    int size;
};

//data struct holding all the info for the operations
struct Data{
    char *comma;
    SymbolTableP table;
    int lookahead;
    int lineno;
    FILE *file;
    int ch;
    int pos;
    char identifier[MAXARRAYSIZE];
    int declsDone;
    int currentReg;
    char assignVar[MAXARRAYSIZE];
    char postfixExpr[MAXARRAYSIZE];
    char fileName[MAXARRAYSIZE];
    FILE *outFile;
};
/*
 * Set up the struct to hold all of the information used by the whole program in the Data struct.
 * Initialize variables.
 */
DataP createData(FILE *file);
/*
 * Create the symbol table and set up the 1st and 2nd elements for begin and end.
 * Return the table to the data struct.
 */
SymbolTableP createTable();
/*
 * Finds an identifier value in the symbol table held in the data struct.
 * If not found, return NOT_FOUND value. Else return the index where the ID was found.
 */
int find(char *value, DataP data);
/*
 * Inserts an identifier into the symbol table held in the data struct.
 */
void insert(char *value, DataP data);
/*
 * Lexan function that processes all of the characters in the program, deciding if
 * it's an ID, NUM, comment, new line or space or tab, or operator.
 * Taken from Dr. Coffey's pseudocode. Returns the character to the parser.
 */
int lexan(DataP data);
/*
 * Reads in all of the characters for each ID (alphas, digits, and _s).
 */
void readID(DataP data);
/*
 * Checks the ID just read in to see if it's legal, as in no consecutive _s and
 * not ending in an _.
 */
int checkIfIDLegal(DataP data);

/*
 * Reads in the int IDs until a semicolon is reached.
 */
void getInts(DataP data);
/*
 * Reads until a , or ; is reached
 */
void getToEnd(DataP data);
/*
 * Reads until next ID
 */
void getToID(DataP data);
/*
 * Reads in the number and saves it into identifier
 */
void readNum(DataP data);
/*
 * exits the program. closes the files, erases any generated code in the output file.
 */
void stopProgram(DataP data);

#endif //RECURSIVEDESCENT_LEXAN_H
