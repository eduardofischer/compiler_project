#include <stdio.h>
#include "../aux/ast.h"

int main() {
  AST_NODE *root = create_node(AST_SYMBOL_TK_PR_INT);
  AST_NODE *child1 = create_node(AST_SYMBOL_TK_PR_CHAR);
  AST_NODE *child2 = create_node(AST_SYMBOL_TK_PR_FLOAT);
  AST_NODE *child3 = create_node(AST_SYMBOL_TK_PR_STRING);
  AST_NODE *child4 = create_node(AST_SYMBOL_TK_IDENTIFICADOR);
  AST_NODE *child5 = create_node(98);

  add_child(root, child1);
  add_child(child1, child2);
  add_child(root, child3);
  add_child(child3, child4);
  add_child(root, child5);

  print_ast(root);

  return 0;
}
