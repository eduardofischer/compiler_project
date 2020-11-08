#ifndef ILOC_CODE_GEN_H
#define ILOC_CODE_GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "types.h"

// Gera o código final do programa
char *generate_iloc_code(INSTRUCTION *last_inst, char *label_main);

// Geração de código de atribuição de variáveis
void gen_code_attribution(PROD_VALUE *var, PROD_VALUE *value);

// Geração de código de identificadores
void gen_code_id(PROD_VALUE *id);

// Geração de código de literais
void gen_code_literal(PROD_VALUE *lit);

// Geração de código para declaração de função
char * gen_code_function_def(PROD_VALUE *func, PROD_VALUE *cmd_list);

// Geração de código de chamada de funçao
void gen_code_func_call(PROD_VALUE *id);

// Gera o código de operações binárias
void gen_code_binary_exp(PROD_VALUE *exp, PROD_VALUE *op1, PROD_VALUE *operator, PROD_VALUE *op2);

// Gera o código de operações lógicas (&& e ||)
void gen_code_logic_op(PROD_VALUE *exp, PROD_VALUE *op1, PROD_VALUE *operator, PROD_VALUE *op2);

// Concatena dois trechos de código
void concat_inst(INSTRUCTION *inst1, INSTRUCTION *inst2);

// Funcoes para remendar os buracos no código intermediário
void *find_holes(INSTRUCTION *last_inst, char *label, PATCH_LIST *list);
void *find_holes_t(INSTRUCTION *last_inst, char *label, PATCH_LIST *list);

// Concatena duas lista de remendos
void concat_hole_list(PATCH_LIST *list1, PATCH_LIST *list2);

// Gera o código para os controladores de fluxo (if, if else, while, for)
void gen_code_if(PROD_VALUE *code, PROD_VALUE *condition, PROD_VALUE *inst);
void gen_code_if_else(PROD_VALUE *code, PROD_VALUE *condition, PROD_VALUE *inst1, PROD_VALUE *inst2);
void gen_code_while(PROD_VALUE *code, PROD_VALUE *condition, PROD_VALUE *inst);
void gen_code_for(PROD_VALUE *code, PROD_VALUE *att1, PROD_VALUE *condition, PROD_VALUE *att2, PROD_VALUE *inst);

// Gera o código de expressões unárias (not)
void gen_code_unary_exp(PROD_VALUE *code, PROD_VALUE *op, PROD_VALUE *exp);

#endif
