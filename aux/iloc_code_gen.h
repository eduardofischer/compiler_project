#ifndef ILOC_CODE_GEN_H
#define ILOC_CODE_GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "types.h"

// Gera o código final do programa
char *generate_iloc_code(INSTRUCTION *last_inst);

// Geração de código de atribuição de variáveis
void gen_code_attribution(PROD_VALUE *var, PROD_VALUE *value);

// Geração de código de identificadores
void gen_code_id(PROD_VALUE *id);

// Geração de código de literais
void gen_code_literal(PROD_VALUE *lit);

// Gera o código de operações binárias
void gen_code_binary_exp(PROD_VALUE *exp, PROD_VALUE *op1, PROD_VALUE *operator, PROD_VALUE *op2);

// Concatena dois trechos de código
void concat_inst(INSTRUCTION *inst1, INSTRUCTION *inst2);

// Funcoes para remendar os buracos no código intermediário
void *find_holes(INSTRUCTION *last_inst, char *rot, LIST *list);
void *find_holes_t(INSTRUCTION *last_inst, char *rot, LIST *list);

// Concatena duas lista de remendos
void concat_hole_list(LIST *list1, LIST *list2);

#endif
