#include <stdio.h>
#include <stdlib.h>
#include "../aux/ast.h"

void *arvore_test;
void exporta_test(void *arvore);


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

  return 0;
}

void exporta_test(void* arvore){
	AST_NODE *root = (AST_NODE*) arvore;
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
        	exporta_test(root->childs[i]);
        }
        fclose(file);
        return;     
}
