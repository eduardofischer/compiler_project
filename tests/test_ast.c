#include <stdio.h>
#include <stdlib.h>
#include "../aux/ast.h"

void *arvore_test;
void exporta_test(void *arvore_test);
void libera_test(void *arvore_test);


int main() {
  AST_NODE *root = create_node("break");
  AST_NODE *child1 = create_node("continue");
  AST_NODE *child2 = create_node("return");
  AST_NODE *child3 = create_node("for");
  AST_NODE *child4 = create_node(">>");

  add_child(root, child1);
  add_child(child1, child2);
  add_child(root, child3);
  add_child(child3, child4);
	
  	
  print_ast(root);
  exporta_test(root);
  libera_test(root);

  return 0;
}

void exporta_test(void* arvore_test){
	AST_NODE *root = (AST_NODE*) arvore_test;
	FILE *file;
	file = fopen ("AST_Exportada.txt", "a");
	
	if (root == NULL)
        	return;
	
        for (int i = 0; i < root->n_children; ++i){
        	fprintf(file,"%p, ", root);
        	fprintf(file,"%p", root->children[i]);
        	fprintf(file,"\n");
        	fprintf(file,"%p [label=%s];", root, root->label);
        	fprintf(file,"\n");
        	fprintf(file,"%p [label=%s];", root->children[i], root->children[i]->label);
        	fprintf(file,"\n");
        	exporta_test(root->children[i]);
        }
        fclose(file);
        return;     
}

void libera_test(void *arvore_test) {
	AST_NODE *root = (AST_NODE*) arvore_test;
	printf("Libera -> ");
	printf("%s\n",root->label);
	
	if (root == NULL)
        	return;
        	
        for (int i = 0; i < root->n_children; ++i){
        	libera_test(root->children[i]);
        }
        
        if(root->n_children == 0){
        	free(root);
        	root = NULL;
        }
        
        return;
}
