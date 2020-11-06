/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>
#include "aux/stack.h"

extern int yyparse(void);
extern int yylex_destroy(void);

STACK_ITEM *table_stack = NULL;
void *arvore = NULL;

void exporta(void *arvore);
void libera(void *arvore);

int main (int argc, char **argv) {
  // Inicializa a tabela de símbolos global
  table_stack = new_scope(table_stack);

  int ret = yyparse();
  
  //exporta(arvore);
  libera(arvore);
  arvore = NULL;
  yylex_destroy();

  // Destroi a tabela de símbolos global
  table_stack = pop(table_stack);
  return ret;
}
