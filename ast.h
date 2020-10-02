#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AST_SYMBOL_TK_PR_INT 1
#define AST_SYMBOL_TK_PR_FLOAT 2
#define AST_SYMBOL_TK_PR_BOOL 3
#define AST_SYMBOL_TK_PR_CHAR 4
#define AST_SYMBOL_TK_PR_STRING 5

// Provavelmente nao vamos precisar de mais que 4 filhos de um nodo
// Obs: se presicar de mais, e so aumentar o numero e fazer mais uma atrivuicao no ast_tree.c
#define MAX_SONS_NEEDED 4

typedef struct ast_node {
    int type;
    struct ast_node * sons[MAX_SONS_NEEDED];
} AST_NODE;

AST_NODE * Ast_Create(int type, AST_NODE * son_0, AST_NODE * son_1, AST_NODE * son_2, AST_NODE * son_3);
void Print_Ast(AST_NODE * node);
