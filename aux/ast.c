#include "ast.h"

// Cria a ast com um numero maximo de 4 filhos no momento
AST_NODE *create_node(int type) {
    AST_NODE *node = (AST_NODE*) calloc(1, sizeof(AST_NODE));
    node->type = type;

    return node;
}

void add_child(AST_NODE *parent, AST_NODE *child){
    parent->n_childs++;
    parent->childs = realloc(parent->childs, parent->n_childs * sizeof(AST_NODE));
    parent->childs[parent->n_childs - 1] = child;
}

// Imprime os nodos da ast recursivamente
// Obs: precisa dos demais casos no momento, so foram implementados os 5 literais
void print_ast_util(AST_NODE *root, int indent_level){
    if(root == NULL)
    	return;

    for(int i=0; i< indent_level; i++) {
        printf("    ");
    }
    printf("↳ ");
    	   	   	   	   	   	
    switch(root->type) {
        case AST_SYMBOL_TK_PR_INT:
            printf("AST_SYMBOL_TK_PR_INT");
            break;
        case AST_SYMBOL_TK_PR_FLOAT:
            printf("AST_SYMBOL_TK_PR_FLOAT");
            break;
        case AST_SYMBOL_TK_PR_BOOL:
            printf("AST_SYMBOL_TK_PR_BOOL");
            break;
        case AST_SYMBOL_TK_PR_CHAR:
            printf("AST_SYMBOL_TK_PR_CHAR");
            break;
        case AST_SYMBOL_TK_PR_STRING:
            printf("AST_SYMBOL_TK_PR_STRING");
            break;
        default:
            printf("AST_UNKNOWN");
            break;
    }

    printf("\n");

    for (int i = 0; i < root->n_childs; ++i)
    	print_ast_util(root->childs[i], indent_level + 1);

    return;
}

void print_ast(AST_NODE *root) {
    print_ast_util(root, 0);
}
