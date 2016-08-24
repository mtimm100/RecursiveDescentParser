/***************************************************************
  Student Name: Mikayla Timm
  File Name: Parser.h
  Assignment number 2

 * This file outlines the functions used by the parser in Parser.c.
***************************************************************/
#ifndef RECURSIVEDESCENT_PARSER_H
#define RECURSIVEDESCENT_PARSER_H

#include "Lexan.h"
#include <stdio.h>

/*
 * Taken from Dr. Coffey's pseudocode.
 * Checks lexan for an ID, calls the expression function to get an assignment statement from the program.
 */
void AssignStmt(DataP data);
/*
 * evaluates expressions by checking term, operator, term. Can be recursively called for nested expressions in factor.
 */
void expression(DataP data);
/*
 * Evaluates a "term" by calling factor and making sure the term is made up of either
 * just an ID or NUM, OR an ID/NUM * or / an ID/NUM, or a nested term with parens.
 */
void term(DataP data);
/*
 * "Factors" the statement by either just confirming there is an ID or NUM where expected, or
 * if there is a nested expression () recursively calls expression and matches closing paren.
 */
void factor(DataP data);
/*
 * Calls lexan to see if character/ID/NUM read in matches "t" passed in. If not, prints an error and closes program.
 */
void match(int t,DataP data);
/*
 * Parses the statements in the program. Reads through until end of file is reached, making calls to other functions
 * to check if all statements are legal. If so, it lists all the IDs in the symbol table. If not, exits with error message.
 */
void parse(DataP data);
/*
 * Removes the final comma from the postfix expression after it's all done
 */
void removeFinalComma(DataP data);
#endif //RECURSIVEDESCENT_PARSER_H
