#include <stdio.h>
#include <stdlib.h>
#include "../aux/hashtable.h"

int main() {
  HT_ENTRY **table1 = malloc(sizeof(HT_ENTRY) * HT_SIZE);
  HT_ENTRY *entry1;

  SYMBOL_ENTRY s1;
  s1.line = 10;
  s1.column = 20;
  s1.literal_type = LIT_TYPE_INT;
  s1.size = sizeof(int);
  s1.arguments = NULL;
  s1.value.i = 30;

  int index = insert_ht_entry(table1, "chave_teste", s1);
  printf("Entrada inserida na tabela na posição %d\n", index);

  entry1 = get_ht_entry(table1, "chave_teste");
  printf("Entrada recuperada na tabela. Chave: %s\n", entry1->key);

  free_ht(table1);

  return 0;
}