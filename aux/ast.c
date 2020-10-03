#include "ast.h"

// Cria node da AST
AST_NODE *create_node(char *label) {
    AST_NODE *node = (AST_NODE*) calloc(1, sizeof(AST_NODE));
    node->label = strdup(label);
    node->valor_lexico = NULL;
    node->n_childs = 0;
    node->childs = NULL;

    return node;
}

// Cria node da AST para literais e identificadores, armazenando seu valor_lexico
AST_NODE *create_node_lex_value(LEX_VALUE *valor_lexico) {
    AST_NODE *node = (AST_NODE*) calloc(1, sizeof(AST_NODE));
    node->valor_lexico = valor_lexico;
    node->label = get_label(valor_lexico);
    node->n_childs = 0;
    node->childs = NULL;

    return node;
}

void add_child(AST_NODE *parent, AST_NODE *child){
    if (parent == NULL) return;
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
        printf("  ");
    }

    printf("↳ %s\n", root->label);

    for (int i = 0; i < root->n_childs; ++i)
    	print_ast_util(root->childs[i], indent_level + 1);

    return;
}

void print_ast(AST_NODE *root) {
    print_ast_util(root, 0);
}

char *get_label(LEX_VALUE *valor_lexico) {
    char *string;
    char str[12];
    if (valor_lexico->token_type == TOKEN_TYPE_LIT) {
        switch(valor_lexico->literal_type) {
            case LIT_TYPE_INT:
                sprintf(str, "%d", valor_lexico->value.i);
                string = strdup(str);
                break;
            case LIT_TYPE_FLOAT:
                gcvt(valor_lexico->value.f, 4, str);
                string = strdup(str);
                break;
            case LIT_TYPE_BOOL:
                if(valor_lexico->value.b == 1){
                    string = strdup("true");
                } else {
                    string = strdup("false");
                }
                break;
            case LIT_TYPE_CHAR:
                str[0] = valor_lexico->value.c;
                str[1] = '\0';
                string = strdup(str);
                break;
            default:
                string = strdup(valor_lexico->value.s);
        }
    } else if (valor_lexico->token_type == TOKEN_TYPE_SPECIAL) {
        str[0] = valor_lexico->value.c;
        str[1] = '\0';
        string = strdup(str);
    } else {
        string = strdup(valor_lexico->value.s);
    }

    return string;
}

void exporta(void* arvore){
	AST_NODE *root = (AST_NODE*) arvore;
	printf("Exporta\n");
	
	FILE *file;
	file = fopen ("AST_Exportada.txt", "a");
	
	if (root == NULL)
        	return;

        for (int i = 0; i < root->n_childs; ++i){
        	fprintf(file,"%p, ", root);
        	fprintf(file,"%p", root->childs[i]);
        	fprintf(file,"\n");
        	fprintf(file,"%p [label=%s];", root, root->label);
        	fprintf(file,"\n");
        	fprintf(file,"%p [label=%s];", root->childs[i], root->childs[i]->label);
        	fprintf(file,"\n");
        	exporta(root->childs[i]);
        }
        fclose(file);
        return;     
}

// Inacabada - talvez dê para usar uma pilha aqui
void libera(void *arvore){
	AST_NODE *root = (AST_NODE*) arvore;
	printf("Libera\n");
	
	if (root == NULL)
        	return;
        if (root->n_childs == 0)
        	free(root);
        	
        for (int i = 0; i < root->n_childs; ++i){
        	libera(root->childs[i]);
        }
        
        return;
        
}

