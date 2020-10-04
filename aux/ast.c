#include "ast.h"

// Cria node da AST
AST_NODE *create_node(char *label) {
    AST_NODE *node = (AST_NODE*) calloc(1, sizeof(AST_NODE));
    node->label = strdup(label);
    node->valor_lexico = NULL;
    node->n_children = 0;
    node->children = NULL;
    return node;
}

// Cria node da AST para literais e identificadores, armazenando seu valor_lexico
AST_NODE *create_node_lex_value(LEX_VALUE valor_lexico) {
    AST_NODE *node = (AST_NODE*) malloc(sizeof(AST_NODE));
    node->valor_lexico = malloc(sizeof(LEX_VALUE));
    node->valor_lexico = memcpy(node->valor_lexico, &valor_lexico, sizeof(LEX_VALUE));
    node->label = get_label(valor_lexico);
    node->n_children = 0;
    node->children = NULL;

    return node;
}

void add_child(AST_NODE *parent, AST_NODE *child){
    if (parent == NULL || child == NULL) return;
    parent->n_children++;
    parent->children = realloc(parent->children, parent->n_children * sizeof(AST_NODE));
    parent->children[parent->n_children - 1] = child;
}

// Imprime os nodos da AST recursivamente
// Obs: precisa dos demais casos no momento, so foram implementados os 5 literais
void print_ast_util(AST_NODE *root, int indent_level){
    if(root == NULL)
    	return;

    for(int i=0; i< indent_level; i++) {
        printf("  ");
    }

    printf("↳ %s [%p]\n", root->label, root);

    for (int i = 0; i < root->n_children; ++i)
    	print_ast_util(root->children[i], indent_level + 1);

    return;
}

void print_ast(AST_NODE *root) {
    print_ast_util(root, 0);
    printf("\n");
}

void concat_label(char **str1, char *str2) {
    *str1 = realloc(*str1, strlen(*str1) + strlen(str2) + 1);
    *str1 = strcat(*str1, str2);
}

char *get_label(LEX_VALUE valor_lexico) {
    char *string;
    char str[12];
    if (valor_lexico.token_type == TOKEN_TYPE_LIT) {
        switch(valor_lexico.literal_type) {
            case LIT_TYPE_INT:
                sprintf(str, "%d", valor_lexico.value.i);
                string = strdup(str);
                break;
            case LIT_TYPE_FLOAT:
                gcvt(valor_lexico.value.f, 4, str);
                string = strdup(str);
                break;
            case LIT_TYPE_BOOL:
                if(valor_lexico.value.b == 1){
                    string = strdup("true");
                } else {
                    string = strdup("false");
                }
                break;
            case LIT_TYPE_CHAR:
                str[0] = valor_lexico.value.c;
                str[1] = '\0';
                string = strdup(str);
                break;
            default:
                string = strdup(valor_lexico.value.s);
        }
    } else if (valor_lexico.token_type == TOKEN_TYPE_SPECIAL) {
        str[0] = valor_lexico.value.c;
        str[1] = '\0';
        string = strdup(str);
    } else {
        string = strdup(valor_lexico.value.s);
    }

    return string;
}

void export_util(void* arvore, char **label_list) {
	AST_NODE *root = (AST_NODE*) arvore;
    int list_length = *label_list != NULL ? strlen(*label_list) : 0;
    int label_length = root->label != NULL ? strlen(root->label) : 0;
    
	if (root == NULL) return;

    *label_list = realloc(*label_list, list_length + label_length + 31);
    sprintf(*label_list + list_length, "%p [label=\"%s\"];\n", root, root->label);

    for (int i = 0; i < root->n_children; ++i){
        printf("%p, ", root);
        printf("%p\n", root->children[i]);
        export_util(root->children[i], label_list);
    }
}

void exporta(void* arvore) {
    char *label_list = NULL;
    export_util(arvore, &label_list);
    printf("\n%s", label_list);
    if (label_list != NULL) free(label_list);
}

void free_node(AST_NODE *node) {
    if (node->valor_lexico != NULL){
        if (node->valor_lexico->literal_type == LIT_TYPE_STRING
            || node->valor_lexico->token_type == TOKEN_TYPE_ID
            || node->valor_lexico->token_type == TOKEN_TYPE_COMPOUND_OP)
            free(node->valor_lexico->value.s);
        free(node->valor_lexico);
    }
    if (node->label != NULL) free(node->label);
    if (node->children != NULL) free(node->children);
    free(node);
}

// Inacabada - talvez dê para usar uma pilha aqui
void libera_util(AST_NODE *node) {
	if (node == NULL) return;
        	
    for (int i = 0; i < node->n_children; ++i)
        libera_util(node->children[i]);

    free_node(node);    
}

void libera(void *arvore) {
    AST_NODE *root = (AST_NODE*) arvore;
    libera_util(root);
}

