#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../types.h"

#define AST_SYMBOL_TK_PR_INT 1
#define AST_SYMBOL_TK_PR_FLOAT 2
#define AST_SYMBOL_TK_PR_BOOL 3
#define AST_SYMBOL_TK_PR_CHAR 4
#define AST_SYMBOL_TK_PR_STRING 5
#define AST_SYMBOL_TK_PR_IF 6
#define AST_SYMBOL_TK_PR_ELSE 7
#define AST_SYMBOL_TK_PR_WHILE 8
#define AST_SYMBOL_TK_PR_DO 9
#define AST_SYMBOL_TK_PR_INPUT 10
#define AST_SYMBOL_TK_PR_OUTPUT 11
#define AST_SYMBOL_TK_PR_RETURN 12
#define AST_SYMBOL_TK_PR_CONST 13
#define AST_SYMBOL_TK_PR_STATIC 14
#define AST_SYMBOL_TK_PR_FOR 15
#define AST_SYMBOL_TK_PR_BREAK 16
#define AST_SYMBOL_TK_PR_CONTINUE 17
#define AST_SYMBOL_TK_OC_LE 18
#define AST_SYMBOL_TK_OC_GE 19
#define AST_SYMBOL_TK_OC_EQ 20
#define AST_SYMBOL_TK_OC_NE 21
#define AST_SYMBOL_TK_OC_AND 22
#define AST_SYMBOL_TK_OC_OR 23
#define AST_SYMBOL_TK_OC_SL 24
#define AST_SYMBOL_TK_OC_SR 25
#define AST_SYMBOL_TK_LIT_INT 26
#define AST_SYMBOL_TK_LIT_FLOAT 27
#define AST_SYMBOL_TK_LIT_FALSE 28
#define AST_SYMBOL_TK_LIT_TRUE 29
#define AST_SYMBOL_TK_LIT_CHAR 30
#define AST_SYMBOL_TK_LIT_STRING 31
#define AST_SYMBOL_TK_IDENTIFICADOR 32
#define AST_SYMBOL_TOKEN_ERRO 33

typedef struct ast_node {
    int type;
    TOKEN_VAL value;
    int n_childs;
    struct ast_node **childs;
} AST_NODE;

AST_NODE *create_node(int type);

void add_child(AST_NODE *parent, AST_NODE *child);

void print_ast(AST_NODE *root);

#endif
