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