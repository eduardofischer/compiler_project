#include "iloc_code_gen.h"

// Retorna um identificador único de registrador no formato rX, sendo X um inteiro
char *new_register() {
  static int n = 0;
  char *reg = malloc(2 + floor(n/10));
  sprintf(reg, "r%d", n++);
  return reg;
}

// Retorna um rótulo único no formato LX, sendo X um inteiro
char *new_label() {
  static int n = 0;
  char *label = malloc(2 + floor(n/10));
  sprintf(label, "L%d", n++);
  return label;
}

// Retorna um ponteiro para uma estrutura do tipo INSTRUCTION
INSTRUCTION *new_instruction(char *code, char *arg1, char *arg2, char *arg3);