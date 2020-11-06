#include "iloc_code_gen.h"

// Retorna um identificador único de registrador no formato rX, sendo X um inteiro
char *_new_register() {
  static int n = 0;
  char *reg = malloc(2 + floor(n/10));
  sprintf(reg, "r%d", n++);
  return reg;
}

// Retorna um rótulo único no formato LX, sendo X um inteiro
char *_new_label() {
  static int n = 0;
  char *label = malloc(2 + floor(n/10));
  sprintf(label, "L%d", n++);
  return label;
}

// Concatena dois trechos de código
char *_concat_code(char *code1, char *code2) {
  char *result = malloc(strlen(code1) + strlen(code2) + 1);
  result = strcat(result, code1);
  free(code1);
  result = strcat(result, code2);
  free(code2);
  return result;
}

// Concatena dois trechos de código
void _concat_inst(INSTRUCTION *inst1, INSTRUCTION *inst2) {
  inst2->prev = inst1;
}

// Retorna um ponteiro para uma estrutura do tipo INSTRUCTION
INSTRUCTION *_new_instruction(char *code, char *arg1, char *arg2, char *arg3) {
  INSTRUCTION *inst = malloc(sizeof(INSTRUCTION));
  inst->code = strdup(code);

  if (arg1 != NULL)
    inst->arg1 = strdup(arg1);
  else
    inst->arg1 = NULL;

  if (arg2 != NULL)
    inst->arg2 = strdup(arg2);
  else
    inst->arg2 = NULL;

  if (arg3 != NULL)
    inst->arg3 = strdup(arg3);
  else
    inst->arg3 = NULL;

  inst->prev = NULL;
  return inst;
}

// Extrai o código ILOC de uma lista do tipo INSTRUCTION
char *extract_code(INSTRUCTION *last_inst) {
  char *code, *prev_code;
  
  if (last_inst->arg2 != NULL) {
    int inst_length = sizeof(last_inst->code) + sizeof(last_inst->arg1) +
      sizeof(last_inst->arg2) + sizeof(last_inst->arg3) + 9;
    code = malloc(inst_length);
    sprintf(code, "%s\t%s, %s => %s\n", last_inst->code, last_inst->arg1,
      last_inst->arg2, last_inst->arg3);
  } else {
    int inst_length = sizeof(last_inst->code) + sizeof(last_inst->arg1) + sizeof(last_inst->arg3) + 9;
    code = malloc(inst_length);
    sprintf(code, "%s\t%s => %s\n", last_inst->code, last_inst->arg1, last_inst->arg3);
  }

  if (last_inst->prev != NULL) {
    prev_code = extract_code(last_inst->prev);
    code = _concat_code(code, prev_code);
  }
  
  return code;
}

// TODO: Geração de código de atribuição de variáveis
void gen_code_attribution(PROD_VALUE *var, PROD_VALUE *value) {
  return;
}

// Geração de código de literais
void gen_code_literal(PROD_VALUE *lit) {
  lit->location = _new_register();

  switch (lit->table_entry.data_type) {
    case DT_INT:
      lit->code = _new_instruction("loadI", lit->table_entry.key, NULL, lit->location);
      break;
  }
}

// Gera o código de operações binárias
void gen_code_binary_exp(PROD_VALUE *exp, PROD_VALUE *op1, PROD_VALUE *operator, PROD_VALUE *op2) {
  exp->location = _new_register();
  if (exp->table_entry.data_type == DT_INT){
    if (strcmp(operator->ast_node->label, "+") == 0)
      exp->code = _new_instruction("add", op1->location, op2->location, exp->location);
    else if (strcmp(operator->ast_node->label, "-") == 0)
      exp->code = _new_instruction("sub", op1->location, op2->location, exp->location);
    else if (strcmp(operator->ast_node->label, "*") == 0)
      exp->code = _new_instruction("mult", op1->location, op2->location, exp->location);
    else if (strcmp(operator->ast_node->label, "/") == 0)
      exp->code = _new_instruction("div", op1->location, op2->location, exp->location);
    
    _concat_inst(exp->code, op2->code);
    _concat_inst(op2->code, op1->code);

    exp->code = op1->code;
  }
}