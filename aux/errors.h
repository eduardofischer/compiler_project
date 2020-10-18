#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "stack.h"
#include "hashtable.h"

#define ERR_UNDECLARED       10
#define ERR_DECLARED         11
#define ERR_VARIABLE         20
#define ERR_VECTOR           21
#define ERR_FUNCTION         22
#define ERR_WRONG_TYPE       30
#define ERR_STRING_TO_X      31
#define ERR_CHAR_TO_X        32
#define ERR_STRING_SIZE      33
#define ERR_MISSING_ARGS     40
#define ERR_EXCESS_ARGS      41
#define ERR_WRONG_TYPE_ARGS  42
#define ERR_WRONG_PAR_INPUT  50
#define ERR_WRONG_PAR_OUTPUT 51
#define ERR_WRONG_PAR_RETURN 52
#define ERR_WRONG_PAR_SHIFT  53

char *get_err_name(int err);

void check_undeclared(char *label, SYMBOL_ENTRY entry);
void check_declared(char *label, SYMBOL_ENTRY symbol);
void check_variable(char *label, SYMBOL_ENTRY symbol);
void check_vector(char *label, SYMBOL_ENTRY symbol);
void check_function(char *label, SYMBOL_ENTRY symbol);
void check_type(char *label, SYMBOL_ENTRY symbol);
void check_input(char *label, SYMBOL_ENTRY symbol);
void check_output(char *label, SYMBOL_ENTRY symbol);
void check_shift(char *label, SYMBOL_ENTRY symbol, LEX_VALUE shift_value);

int infer_type(SYMBOL_ENTRY s1, SYMBOL_ENTRY s2);

void throw_error(int err, char *label, SYMBOL_ENTRY entry);

#endif