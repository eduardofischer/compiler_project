#ifndef ILOC_CODE_GEN_H
#define ILOC_CODE_GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Estrutura que representa um lista encadeaada (invertida) de instruções
typedef struct instruction {
  char *code;
  char *arg1;
  char *arg2;
  char *arg3;
  struct instruction *previous;
} INSTRUCTION;

// Retorna um identificador único de registrador no formato rX, sendo X um inteiro
char *new_register();

// Retorna um rótulo único no formato LX, sendo X um inteiro
char *new_label();

// Retorna um ponteiro para uma estrutura do tipo INSTRUCTION
INSTRUCTION *new_instruction(char *code, char *arg1, char *arg2, char *arg3);

#endif
