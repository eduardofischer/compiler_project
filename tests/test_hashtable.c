#include "../aux/hashtable.h"

int main() {
  SYMBOL_ENTRY **table1 = malloc(sizeof(SYMBOL_ENTRY) * HT_SIZE);
  SYMBOL_ENTRY *entry1;

  char key[20] = "testechave";
  SYMBOL_ENTRY s1;
  s1.line = 10;
  s1.column = 20;
  s1.data_type = DT_INT;
  s1.size = sizeof(int);
  s1.arguments = NULL;
  s1.value.i = 30;

  int index = insert_ht_entry(table1, key, s1);
  printf("Entrada inserida na tabela na posição %d\n", index);

  entry1 = get_ht_entry(table1, key);
  printf("Entrada recuperada na tabela. Chave: %s\n", entry1->key);

  free_ht(table1);

  return 0;
}