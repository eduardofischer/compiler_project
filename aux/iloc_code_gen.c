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
void concat_inst(INSTRUCTION *inst1, INSTRUCTION *inst2) {
  INSTRUCTION *first = inst2;
  
  while (first->prev != NULL)
    first = first->prev;
    
  first->prev = inst1;
}

// Retorna um ponteiro para uma estrutura do tipo INSTRUCTION
INSTRUCTION *_new_instruction(char *code, char *arg1, char *arg2, char *arg3, char *label) {
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
  
  if (label != NULL)
    inst->label = strdup(label);
  else
    inst->label = NULL;

  inst->prev = NULL;
  return inst;
}

// Extrai o código ILOC de uma lista do tipo INSTRUCTION
char *_extract_code(INSTRUCTION *last_inst) {
  char *code, *prev_code, *temp_code;

  // Instruções sem argumentos
  if (last_inst->arg1 == NULL && last_inst->arg2 == NULL && last_inst->arg3 == NULL){
    int inst_length = strlen(last_inst->code) + 3;
    temp_code = malloc(inst_length);
    sprintf(temp_code, "\t%s\n", last_inst->code);
  }
  // Instruções com dois destinos e flecha dupla
  else if (!strcmp(last_inst->code, "storeAI")) {
    if (last_inst->arg3 != NULL) {
      int inst_length = strlen(last_inst->code) + strlen(last_inst->arg1) + strlen(last_inst->arg2) + strlen(last_inst->arg3) + 10;
      temp_code = malloc(inst_length);
      sprintf(temp_code, "\t%s\t%s => %s, %s\n", last_inst->code, last_inst->arg1, last_inst->arg2, last_inst->arg3);
    } else {
      int inst_length = strlen(last_inst->code) + strlen(last_inst->arg1) + strlen(last_inst->arg2) + 8;
      temp_code = malloc(inst_length);
      sprintf(temp_code, "\t%s\t%s => %s\n", last_inst->code, last_inst->arg1, last_inst->arg2);
    }
  }
  // Instruções com dois destinos e flecha simples
  else if (!strcmp(last_inst->code, "cbr")) {
    if (last_inst->arg3 != NULL) {
      int inst_length = strlen(last_inst->code) + strlen(last_inst->arg1) + strlen(last_inst->arg2) + strlen(last_inst->arg3) + 10;
      temp_code = malloc(inst_length);
      sprintf(temp_code, "\t%s\t%s -> %s, %s\n", last_inst->code, last_inst->arg1, last_inst->arg2, last_inst->arg3);
    } else {
      int inst_length = strlen(last_inst->code) + strlen(last_inst->arg1) + strlen(last_inst->arg2) + 8;
      temp_code = malloc(inst_length);
      sprintf(temp_code, "\t%s\t%s -> %s\n", last_inst->code, last_inst->arg1, last_inst->arg2);
    }
  } 
  // Gera instruções com 2 origens e flecha simples
  else if (!strcmp(last_inst->code, "cmp_LT") ||
           !strcmp(last_inst->code, "cmp_LE") || 
           !strcmp(last_inst->code, "cmp_EQ") || 
           !strcmp(last_inst->code, "cmp_GE") || 
           !strcmp(last_inst->code, "cmp_GT") || 
           !strcmp(last_inst->code, "cmp_NE") || 
           !strcmp(last_inst->code, "jump") || 
           !strcmp(last_inst->code, "jumpI")) { 
    if (last_inst->arg1 == NULL && last_inst->arg2 == NULL) {
      int inst_length = strlen(last_inst->code)  + strlen(last_inst->arg3) + 9;
      temp_code = malloc(inst_length);
      sprintf(temp_code, "\t%s\t\t -> %s\n", last_inst->code, last_inst->arg3);
    } else if (last_inst->arg2 != NULL) {
      int inst_length = strlen(last_inst->code) + strlen(last_inst->arg1) + strlen(last_inst->arg2) + strlen(last_inst->arg3) + 10;
      temp_code = malloc(inst_length);
      sprintf(temp_code, "\t%s\t%s, %s -> %s\n", last_inst->code, last_inst->arg1, last_inst->arg2, last_inst->arg3);
    } else {
      int inst_length = strlen(last_inst->code) + strlen(last_inst->arg1) + strlen(last_inst->arg3) + 8;
      temp_code = malloc(inst_length);
      sprintf(temp_code, "\t%s\t%s -> %s\n", last_inst->code, last_inst->arg1, last_inst->arg3);
    }
  }
  // Gera instruções com 2 origens e flecha dupla
  else { 
    if (last_inst->arg2 != NULL) {
      int inst_length = strlen(last_inst->code) + strlen(last_inst->arg1) + strlen(last_inst->arg2) + strlen(last_inst->arg3) + 10;
      temp_code = malloc(inst_length);
      sprintf(temp_code, "\t%s\t%s, %s => %s\n", last_inst->code, last_inst->arg1, last_inst->arg2, last_inst->arg3);
    } else {
      int inst_length = strlen(last_inst->code) + strlen(last_inst->arg1) + strlen(last_inst->arg3) + 8;
      temp_code = malloc(inst_length);
      sprintf(temp_code, "\t%s\t%s => %s\n", last_inst->code, last_inst->arg1, last_inst->arg3);
    }
  }
  
  // Adição da label à instrução
  if (last_inst->label != NULL) {
    code = malloc(strlen(temp_code) + strlen(last_inst->label) + 2);
    sprintf(code, "%s:%s", last_inst->label, temp_code);
    free(temp_code);
  } else {
    code = temp_code;
  }
  
  // Concatenação recursiva de instruções
  if (last_inst->prev != NULL) {
    prev_code = _extract_code(last_inst->prev);
    code = _concat_code(prev_code, code);
  }
  
  return code;
}

// Retorna o tamanho do programa
int _get_program_size(INSTRUCTION *last_inst) {
  int size = 1;
  INSTRUCTION *inst = last_inst;
  
  while (inst->prev != NULL) {
    inst = inst->prev;
    size++;
  }
  
  return size;
}

// Gera o código final do programa
char *generate_iloc_code(INSTRUCTION *last_inst) {
  INSTRUCTION *inst_list, *inst_aux = last_inst;
  char str_size[12];
  printf(str_size, "%d", _get_program_size(last_inst) + 4);

  // Adiciona instrução de HALT
  inst_list = _new_instruction("halt", NULL, NULL, NULL, NULL);
  concat_inst(inst_aux, inst_list);

  // Inicializa registradores auxiliares
  inst_aux = _new_instruction("loadI", str_size, NULL, "rbss", NULL);
  concat_inst(inst_aux, inst_list);
  inst_aux = _new_instruction("loadI", RFP_INIT_VALUE, NULL, "rsp", NULL);
  concat_inst(inst_aux, inst_list);
  inst_aux = _new_instruction("loadI", RFP_INIT_VALUE, NULL, "rfp", NULL);
  concat_inst(inst_aux, inst_list);

  return _extract_code(inst_list);
}


// Geração de código de identificadores
void gen_code_id(PROD_VALUE *id) {
  id->location = _new_register();

  char str_offset[12];
  sprintf(str_offset, "%d", id->table_entry.offset);

  switch (id->table_entry.entry_type) {
    case ET_VARIABLE:
      id->code = _new_instruction("loadAI", "rfp", str_offset, id->location, NULL);
      break;
    default:
      id->code = NULL;
  }
}

// Geração de código de literais
void gen_code_literal(PROD_VALUE *lit) {
  lit->location = _new_register();

  switch (lit->table_entry.data_type) {
    case DT_INT:
      lit->code = _new_instruction("loadI", lit->table_entry.key, NULL, lit->location, NULL);
      break;
    default:
      lit->code = NULL;
  }
}

// Geração de código de atribuição de variáveis
void gen_code_attribution(PROD_VALUE *var, PROD_VALUE *value) {
  var->location = NULL;
  if (value->location != NULL) {
    char str_offset[12];
    sprintf(str_offset, "%d", var->table_entry.offset);
    var->code = _new_instruction("storeAI", value->location, "rfp", str_offset, NULL);
  }

  concat_inst(value->code, var->code);
}

// TODO: Geração de código de chamada de funçao
void gen_code_func_call(PROD_VALUE *id) {
  return;
}

void _patch_holes(PATCH_LIST *list, char *label) {
  PATCH_LIST *hole = list, *temp;

  do {
    *(hole->label) = label;
    temp = hole->next;
    free(hole);
    hole = temp;
  } while (hole != NULL);
}

PATCH_LIST *_new_patch_list(char **addr) {
  PATCH_LIST *list = malloc(sizeof(PATCH_LIST));
  list->label = addr;
  list->next = NULL;
  return list;
}

PATCH_LIST *_insert_patch_list(PATCH_LIST *list, char **addr) {
  PATCH_LIST *new_patch = malloc(sizeof(PATCH_LIST));
  new_patch->label = addr;
  new_patch->next = list;
  return new_patch;
}

PATCH_LIST *_concat_patch_list(PATCH_LIST *list1, PATCH_LIST *list2) {
  PATCH_LIST *last = list1;
  while (last->next != NULL)
    last = last->next;
  last->next = list2;
  return list1;
}

// Gera o código de operações binárias
void gen_code_binary_exp(PROD_VALUE *exp, PROD_VALUE *op1, PROD_VALUE *operator, PROD_VALUE *op2) {
  if (op1->location != NULL && op2->location != NULL) {
    exp->location = _new_register();
    
    if (!strcmp(operator->ast_node->label, "+"))
      exp->code = _new_instruction("add", op1->location, op2->location, exp->location, NULL);
    else if (!strcmp(operator->ast_node->label, "-"))
      exp->code = _new_instruction("sub", op1->location, op2->location, exp->location, NULL);
    else if (!strcmp(operator->ast_node->label, "*"))
      exp->code = _new_instruction("mult", op1->location, op2->location, exp->location, NULL);
    else if (!strcmp(operator->ast_node->label, "/"))
      exp->code = _new_instruction("div", op1->location, op2->location, exp->location, NULL);
    else if (!strcmp(operator->ast_node->label, "<")){
      INSTRUCTION *inst1 = _new_instruction("cmp_LT", op1->location, op2->location, exp->location, NULL);
      exp->code = _new_instruction("cbr", exp->location, "HOLE", "HOLE", NULL);
      exp->tl = _new_patch_list(&exp->code->arg2);
      exp->fl = _new_patch_list(&exp->code->arg3);
      concat_inst(inst1, exp->code);
    }
    else if (!strcmp(operator->ast_node->label, ">")){
      INSTRUCTION *inst1 = _new_instruction("cmp_GT", op1->location, op2->location, exp->location, NULL);
      exp->code = _new_instruction("cbr", exp->location, "HOLE", "HOLE", NULL);
      exp->tl = _new_patch_list(&exp->code->arg2);
      exp->fl = _new_patch_list(&exp->code->arg3);
      concat_inst(inst1, exp->code);
    }
    else if (!strcmp(operator->ast_node->label, "<=")){
      INSTRUCTION *inst1 = _new_instruction("cmp_LE", op1->location, op2->location, exp->location, NULL);
      exp->code = _new_instruction("cbr", exp->location, "HOLE", "HOLE", NULL);
      exp->tl = _new_patch_list(&exp->code->arg2);
      exp->fl = _new_patch_list(&exp->code->arg3);
      concat_inst(inst1, exp->code);
    }
    else if (!strcmp(operator->ast_node->label, ">=")){
      INSTRUCTION *inst1 = _new_instruction("cmp_GE", op1->location, op2->location, exp->location, NULL);
      exp->code = _new_instruction("cbr", exp->location, "HOLE", "HOLE", NULL);
      exp->tl = _new_patch_list(&exp->code->arg2);
      exp->fl = _new_patch_list(&exp->code->arg3);
      concat_inst(inst1, exp->code);
    }
    else if (!strcmp(operator->ast_node->label, "==")){
      INSTRUCTION *inst1 = _new_instruction("cmp_EQ", op1->location, op2->location, exp->location, NULL);
      exp->code = _new_instruction("cbr", exp->location, "HOLE", "HOLE", NULL);
      exp->tl = _new_patch_list(&exp->code->arg2);
      exp->fl = _new_patch_list(&exp->code->arg3);
      concat_inst(inst1, exp->code);
    }
    else if (!strcmp(operator->ast_node->label, "!=")){
      INSTRUCTION *inst1 = _new_instruction("cmp_NE", op1->location, op2->location, exp->location, NULL);
      exp->code = _new_instruction("cbr", exp->location, "HOLE", "HOLE", NULL);
      exp->tl = _new_patch_list(&exp->code->arg2);
      exp->fl = _new_patch_list(&exp->code->arg3);
      concat_inst(inst1, exp->code);
    }
    
    concat_inst(op1->code, op2->code);
    concat_inst(op2->code, exp->code);
  }
}

// Gera o código de operações lógicas (&& e ||)
void gen_code_logic_op(PROD_VALUE *exp, PROD_VALUE *op1, PROD_VALUE *operator, PROD_VALUE *op2) {
  INSTRUCTION *inst_aux1, *inst_aux2;
  char *label_true = _new_label();
  char *label_false = _new_label();
  char *label_end = _new_label();
  exp->location = _new_register();

  if (!strcmp(operator->ast_node->label, "||")){
    char *label = _new_label();
    _patch_holes(op1->fl, label);
    exp->code = _new_instruction("nop", NULL, NULL, NULL, label);
    exp->tl = _concat_patch_list(op1->tl, op2->tl);
    exp->fl = op2->fl;
  } else if (!strcmp(operator->ast_node->label, "&&")){
    char *label = _new_label();
    _patch_holes(op1->tl, label);
    exp->code = _new_instruction("nop", NULL, NULL, NULL, label);
    exp->tl = op2->tl;
    exp->fl = _concat_patch_list(op1->fl, op2->fl);
  }
  concat_inst(op1->code, exp->code);
  concat_inst(exp->code, op2->code);

  _patch_holes(exp->tl, label_true);
  _patch_holes(exp->fl, label_false);

  inst_aux1 = _new_instruction("loadI", "1", NULL, exp->location, label_true);
  concat_inst(op2->code, inst_aux1);
  inst_aux2 = _new_instruction("jumpI", NULL, NULL, label_end, NULL);
  concat_inst(inst_aux1, inst_aux2);
  inst_aux1 = _new_instruction("loadI", "0", NULL, exp->location, label_false);
  concat_inst(inst_aux2, inst_aux1);
  inst_aux2 = _new_instruction("jumpI", NULL, NULL, label_end, NULL);
  concat_inst(inst_aux1, inst_aux2);
  inst_aux1 = _new_instruction("nop", NULL, NULL, NULL, label_end);
  concat_inst(inst_aux2, inst_aux1);
  exp->code = inst_aux1;
}

// Gera o código do controlador de fluxo if
void gen_code_if(PROD_VALUE *code, PROD_VALUE *condition, PROD_VALUE *inst) {
  INSTRUCTION *inst_aux1, *inst_aux2;
  code->location = _new_register();
  char *label_true = _new_label();
  char *label_false = _new_label();

  _patch_holes(condition->tl, label_true);
  _patch_holes(condition->fl, label_false);

  inst_aux1 = _new_instruction("nop", NULL, NULL, NULL, label_true);
  concat_inst(condition->code, inst_aux1);
  concat_inst(inst_aux1, inst->code);
  inst_aux2 = _new_instruction("nop", NULL, NULL, NULL, label_false);
  concat_inst( inst->code, inst_aux2);
  code->code = inst_aux2;
}

// Gera o código do controlador de fluxo if else
void gen_code_if_else(PROD_VALUE *code, PROD_VALUE *condition, PROD_VALUE *inst1, PROD_VALUE *inst2) {
  INSTRUCTION *inst_aux1, *inst_aux2, *inst_aux3, *inst_aux4, *inst_aux5;
  code->location = _new_register();
  char *label_true = _new_label();
  char *label_false = _new_label();
  char *label_end = _new_label();

  _patch_holes(condition->tl, label_true);
  _patch_holes(condition->fl, label_false);

  inst_aux1 = _new_instruction("nop", NULL, NULL, NULL, label_true);
  concat_inst(condition->code, inst_aux1);
  concat_inst(inst_aux1, inst1->code);

  inst_aux2 = _new_instruction("jumpI", NULL, NULL, label_end, NULL);
  concat_inst(inst1->code, inst_aux2);
  inst_aux3 = _new_instruction("nop", NULL, NULL, NULL, label_false);
  concat_inst(inst_aux2, inst_aux3);
  concat_inst(inst_aux3, inst2->code);
  inst_aux4 = _new_instruction("jumpI", NULL, NULL, label_end, NULL);
  concat_inst(inst2->code, inst_aux4);
  inst_aux5 = _new_instruction("nop", NULL, NULL, NULL, label_end);
  concat_inst(inst_aux4, inst_aux5);
  code->code = inst_aux5;
}