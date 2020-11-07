#ifndef ILOC_CODE_GEN_H
#define ILOC_CODE_GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "types.h"

// Geração de código de atribuição de variáveis
void gen_code_attribution(PROD_VALUE *var, PROD_VALUE *value);

// Geração de código de identificadores
void gen_code_id(PROD_VALUE *id);

// Geração de código de literais
void gen_code_literal(PROD_VALUE *lit);

// Gera o código de operações binárias
void gen_code_binary_exp(PROD_VALUE *exp, PROD_VALUE *op1, PROD_VALUE *operator, PROD_VALUE *op2);

// Extrai o código ILOC de uma lista do tipo INSTRUCTION
char *extract_code(INSTRUCTION *last_inst);

// Concatena dois trechos de código
void concat_inst(INSTRUCTION *inst1, INSTRUCTION *inst2);

#endif
