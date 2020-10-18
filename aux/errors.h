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

char *print_err_msg(int err, SYMBOL_ENTRY entry);

void check_undeclared(SYMBOL_ENTRY entry);
void check_declared(SYMBOL_ENTRY symbol);
void check_variable(SYMBOL_ENTRY symbol);
void check_vector(SYMBOL_ENTRY symbol);
void check_function(SYMBOL_ENTRY symbol);
void check_type(char *label, SYMBOL_ENTRY symbol);
void check_input(SYMBOL_ENTRY symbol);
void check_output(SYMBOL_ENTRY symbol);
void check_shift(SYMBOL_ENTRY symbol, LEX_VALUE shift_value);
void check_string_size(SYMBOL_ENTRY string1, int string2_size);
int check_is_string_op(char *label, int data_type_arg1, int data_type_arg2);
void check_return(SYMBOL_ENTRY id, int data_type_return);

int infer_type(SYMBOL_ENTRY s1, SYMBOL_ENTRY s2);

void throw_error(int err, SYMBOL_ENTRY entry);

SYMBOL_ENTRY *find_table_entry(STACK_ITEM *table_stack, char *key);

#endif