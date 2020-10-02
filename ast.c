#include "ast.h"

// Cria a ast com um numero maximo de 4 filhos no momento
AST_NODE * Ast_Create(int type, AST_NODE * son_0, AST_NODE * son_1, AST_NODE * son_2, AST_NODE * son_3) {
    AST_NODE * node = (AST_NODE * ) calloc(1, sizeof(AST_NODE));
    node -> type = type;
    node -> sons[0] = son_0;
    node -> sons[1] = son_1;
    node -> sons[2] = son_2;
    node -> sons[3] = son_3;

    return node;
}

// Imprime os nodos da ast recursivamente
// Obs: precisa dos demais casos no momento, so foram implementados os 5 literais
void Print_Ast(AST_NODE * node){ 
    if (node == 0)
    	return;
    	
    switch (node -> type) {
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

    for (int i = 0; i < MAX_SONS_NEEDED; ++i)
    	Print_Ast(node -> sons[i]);

    return;
}
