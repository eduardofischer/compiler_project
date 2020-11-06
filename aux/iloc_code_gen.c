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
  int result_size = strlen(code1) + strlen(code2) + 1;
  char *result = malloc(result_size);
  result = memset(result, 0, result_size);
  result = strcat(result, code1);
  free(code1);
  result = strcat(result, code2);
  free(code2);
  return result;
}

// Concatena dois trechos de código
void _concat_inst(INSTRUCTION *inst1, INSTRUCTION *inst2) {
  INSTRUCTION *first = inst2;
  
  while (first->prev != NULL)
    first = first->prev;
    
  first->prev = inst1;
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
    code = _concat_code(prev_code, code);
  }
  
  return code;
}

// TODO: Geração de código de atribuição de variáveis
void gen_code_attribution(PROD_VALUE *var, PROD_VALUE *value) {
  var->location = NULL;
  if (value->location != NULL) {
    char str_offset[12];
    sprintf(str_offset, "%d", var->table_entry.offset);
    var->code = _new_instruction("storeAI", value->location, "rfp", str_offset);
  }

  _concat_inst(value->code, var->code);
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

// Faz um remendo
void _make_patch(INSTRUCTION *inst1, char *rot) {
  inst1->arg3 = rot;
  // find_holes();
}

// Gera buraco que deve ser remendado no futuro
char* _make_hole(){
  static int n = 0;
  char *label = malloc(2 + floor(n/10));
  sprintf(label, "Remendo%d", n++);
  return label;
}

// Gera o código de operações binárias
void gen_code_binary_exp(PROD_VALUE *exp, PROD_VALUE *op1, PROD_VALUE *operator, PROD_VALUE *op2) {
  if (exp->table_entry.data_type == DT_INT) {
    exp->location = _new_register();
    
    if (strcmp(operator->ast_node->label, "+") == 0)
      exp->code = _new_instruction("add", op1->location, op2->location, exp->location);
    else if (strcmp(operator->ast_node->label, "-") == 0)
      exp->code = _new_instruction("sub", op1->location, op2->location, exp->location);
    else if (strcmp(operator->ast_node->label, "*") == 0)
      exp->code = _new_instruction("mult", op1->location, op2->location, exp->location);
    else if (strcmp(operator->ast_node->label, "/") == 0)
      exp->code = _new_instruction("div", op1->location, op2->location, exp->location);
    else if (strcmp(operator->ast_node->label, "||") == 0){
      char *rot = _new_label();
      _make_patch(exp->code, rot);
    }
    else if (strcmp(operator->ast_node->label, "<") == 0){
      INSTRUCTION *inst1 = _new_instruction("cmp_LT", op1->location, op2->location, exp->location);
      exp->code = _new_instruction("cbr", exp->location, _make_hole(), _make_hole());
      
      _concat_inst(inst1, exp->code);
    }
    
    _concat_inst(op1->code, op2->code);
    _concat_inst(op2->code, exp->code);
  }
}


