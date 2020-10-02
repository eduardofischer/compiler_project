#include "ast.h"

// Cria node da AST
AST_NODE *create_node(int type) {
    AST_NODE *node = (AST_NODE*) calloc(1, sizeof(AST_NODE));
    node->type = type;
    node->n_childs = 0;
    node->childs = NULL;

    return node;
}

void add_child(AST_NODE *parent, AST_NODE *child){
    parent->n_childs++;
    parent->childs = realloc(parent->childs, parent->n_childs * sizeof(AST_NODE));
    parent->childs[parent->n_childs - 1] = child;
}

// Imprime os nodos da AST recursivamente
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
        case AST_SYMBOL_TK_PR_IF:  
            printf("AST_SYMBOL_TK_PR_IF");
            break;
        case AST_SYMBOL_TK_PR_ELSE:
            printf("AST_SYMBOL_TK_PR_ELSE");
            break;
        case AST_SYMBOL_TK_PR_WHILE:
            printf("AST_SYMBOL_TK_PR_WHILE");
            break;
        case AST_SYMBOL_TK_PR_DO:
            printf("AST_SYMBOL_TK_PR_DO");
            break;
        case AST_SYMBOL_TK_PR_INPUT:
            printf("AST_SYMBOL_TK_PR_INPUT");
            break;
        case AST_SYMBOL_TK_PR_OUTPUT:  
            printf("AST_SYMBOL_TK_PR_OUTPUT");
            break;
        case AST_SYMBOL_TK_PR_RETURN:
            printf("AST_SYMBOL_TK_PR_RETURN");
            break;
        case AST_SYMBOL_TK_PR_CONST:
            printf("AST_SYMBOL_TK_PR_CONST");
            break;
        case AST_SYMBOL_TK_PR_STATIC:
            printf("AST_SYMBOL_TK_PR_STATIC");
            break;
        case AST_SYMBOL_TK_PR_FOR:
            printf("AST_SYMBOL_TK_PR_FOR");
            break;
        case AST_SYMBOL_TK_PR_BREAK:
            printf("AST_SYMBOL_TK_PR_BREAK");
            break;
        case AST_SYMBOL_TK_PR_CONTINUE:
            printf("AST_SYMBOL_TK_PR_CONTINUE");
            break;
        case AST_SYMBOL_TK_OC_LE:
            printf("AST_SYMBOL_TK_OC_LE");
            break;
        case AST_SYMBOL_TK_OC_GE:
            printf("AST_SYMBOL_TK_OC_GE");
            break;
        case AST_SYMBOL_TK_OC_EQ:
            printf("AST_SYMBOL_TK_OC_EQ");
            break;      
        case AST_SYMBOL_TK_OC_NE:
            printf("AST_SYMBOL_TK_OC_NE");
            break;
        case AST_SYMBOL_TK_OC_AND:
            printf("AST_SYMBOL_TK_OC_AND");
            break;
        case AST_SYMBOL_TK_OC_OR:
            printf("AST_SYMBOL_TK_OC_OR");
            break;
        case AST_SYMBOL_TK_OC_SL:
            printf("AST_SYMBOL_TK_OC_SL");
            break;
        case AST_SYMBOL_TK_OC_SR:
            printf("AST_SYMBOL_TK_OC_SR");
            break;
        case AST_SYMBOL_TK_LIT_INT:
            printf("AST_SYMBOL_TK_LIT_INT");
            break;
        case AST_SYMBOL_TK_LIT_FLOAT:
            printf("AST_SYMBOL_TK_LIT_FLOAT");
            break;
        case AST_SYMBOL_TK_LIT_FALSE:
            printf("AST_SYMBOL_TK_LIT_FALSE");
            break;
        case AST_SYMBOL_TK_LIT_TRUE:
            printf("AST_SYMBOL_TK_LIT_TRUE");
            break;
        case AST_SYMBOL_TK_LIT_CHAR:
            printf("AST_SYMBOL_TK_LIT_CHAR");
            break;
        case AST_SYMBOL_TK_LIT_STRING:
            printf("AST_SYMBOL_TK_LIT_STRING");
            break;
        case AST_SYMBOL_TK_IDENTIFICADOR:
            printf("AST_SYMBOL_TK_IDENTIFICADOR");
            break;
        case AST_SYMBOL_TOKEN_ERRO:
            printf("AST_SYMBOL_TOKEN_ERRO");
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
