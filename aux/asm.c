#include "asm.h"

// Gera o código assembly x86_64 a partir do código intermediário ILOC
char *generate_asm(INSTRUCTION *inst) {
  char *prev = NULL, *current = NULL, *asm_code = NULL;
  if (inst->prev != NULL)
    prev = generate_asm(inst->prev);

  current = ILOCtoX86(inst);

  if (current == NULL) {
    printf("Instrução %s não traduzida.\n", inst->code);
    return prev;
  }

  asm_code = current;

  if (prev != NULL) {
    asm_code = realloc(asm_code, strlen(prev) + strlen(current) + 1);
    strcat(asm_code, prev);
  }

  return asm_code;
}

// Traduz uma instrução ILOC para x86_64
char *ILOCtoX86(INSTRUCTION *inst) {
  char *x86_inst = NULL;
  if (!strcmp(inst->code, "nop")) {
    x86_inst = malloc(5);
    sprintf(x86_inst, "NOP\n");
  } else if (!strcmp(inst->code, "addI")) {
    x86_inst = malloc(6);
    sprintf(x86_inst, "addI\n"); // TODO
  }

  return x86_inst;
}