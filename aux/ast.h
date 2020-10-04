#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../types.h"

typedef struct ast_node {
    char *label;
    LEX_VALUE *valor_lexico;
    int n_children;
    struct ast_node **children;
} AST_NODE;

AST_NODE *create_node(char *label);

AST_NODE *create_node_lex_value(LEX_VALUE *valor_lexico);

void add_child(AST_NODE *parent, AST_NODE *child);

void print_ast(AST_NODE *root);

void concat_label(char **str1, char *str2);

char *get_label(LEX_VALUE *valor_lexico);

void free_node(AST_NODE *node);

void libera(void *arvore);

void exporta(void* arvore);
void libera(void *arvore);
#endif
