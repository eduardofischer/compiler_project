#include "../aux/stack.h"

int main() {
  STACK_ITEM *stack = new_scope(stack);
  HT_ENTRY **table1 = top(stack);

  char key1[20] = "chave1";
  SYMBOL_ENTRY s1;
  s1.line = 10;
  s1.column = 20;
  s1.data_type = DT_INT;
  s1.size = sizeof(int);
  s1.arguments = NULL;
  s1.value.i = 30;
  int index1 = insert_ht_entry(table1, key1, s1);
  printf("Entrada inserida na tabela na posição %d\n", index1);

  stack = new_scope(stack);
  HT_ENTRY **table2 = top(stack);

  char key2[20] = "chave2";
  SYMBOL_ENTRY s2;
  s2.line = 10;
  s2.column = 20;
  s2.data_type = DT_INT;
  s2.size = sizeof(int);
  s2.arguments = NULL;
  s2.value.i = 30;

  int index2 = insert_ht_entry(table2, key2, s2);
  printf("Entrada inserida na tabela na posição %d\n", index2);

  HT_ENTRY **t2 = top(stack);
  HT_ENTRY *entry2 = get_ht_entry(t2, key2);
  printf("Entrada recuperada na tabela 2. Chave: %s\n", entry2->key);
  stack = pop(stack);
  HT_ENTRY **t1 = top(stack);
  HT_ENTRY *entry1 = get_ht_entry(t1, key1);
  printf("Entrada recuperada na tabela 1. Chave: %s\n", entry1->key);

  stack = pop(stack);

  return 0;
}