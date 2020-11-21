#include "asm.h"
int globl_counter = -4;

// Gera o código assembly x86_64 a partir do código intermediário ILOC
char *generate_asm(INSTRUCTION *inst) {
  char *prev = NULL, *current = NULL, *asm_code = NULL;
  if (inst->prev != NULL){
    if (!strcmp(inst->code, "loadI")){
      globl_counter -= 4;
    }
    prev = generate_asm(inst->prev);
  }

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
  printf("%s", asm_code);
  return "";
}

// Traduz uma instrução ILOC para x86_64
char *ILOCtoX86(INSTRUCTION *inst) {
  char *x86_inst = NULL;
  if (!strcmp(inst->code, "nop")) {
    x86_inst = malloc(5);
    sprintf(x86_inst, "NOP\n");
  } else if (!strcmp(inst->code, "addI")) {
    x86_inst = malloc(6);
    sprintf(x86_inst, "\taddI\n"); // TODO
  } else if (!strcmp(inst->code, "loadI"))
  {
    int inst_length = strlen("movl")  + strlen(inst->arg1) + 20;
    x86_inst = malloc(inst_length);
    sprintf(x86_inst, "\tmovl\t$%s => %d(%%rbp)\n", inst->arg1, globl_counter);
    globl_counter += 4;
  }
  

  return x86_inst;
}