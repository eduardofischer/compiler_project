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

  if (inst2 == NULL) {
    inst2 = malloc(sizeof(INSTRUCTION));
    inst2->arg1 = inst1->arg1;
    inst2->arg2 = inst1->arg2;
    inst2->arg3 = inst1->arg3;
    inst2->code = inst1->code;
    inst2->label = inst1->label;
    inst2->prev = inst1->prev;
  } else {
    while (first->prev != NULL)
      first = first->prev;
    first->prev = inst1;
  }
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
  // Instruções com dois destinos
  else if (!strcmp(last_inst->code, "storeAI") ||
           !strcmp(last_inst->code, "cbr")) {
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
  // Gera instruções com 2 origens
  else { 
    if (last_inst->arg1 == NULL && last_inst->arg2 == NULL) {
      int inst_length = strlen(last_inst->code)  + strlen(last_inst->arg3) + 7;
      temp_code = malloc(inst_length);
      sprintf(temp_code, "\t%s\t=> %s\n", last_inst->code, last_inst->arg3);
    } else if (last_inst->arg2 != NULL) {
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
char *generate_iloc_code(INSTRUCTION *last_inst, char *label_main) {
  INSTRUCTION *inst_aux1=NULL, *inst_aux2=NULL;
  char str_size[12];
  sprintf(str_size, "%d", _get_program_size(last_inst) + 4);

  // Inicializa registradores auxiliares
  inst_aux1 = _new_instruction("loadI", RFP_INIT_VALUE, NULL, "rfp", NULL);
  inst_aux2 = _new_instruction("loadI", RFP_INIT_VALUE, NULL, "rsp", NULL);
  concat_inst(inst_aux1, inst_aux2);
  inst_aux1 = _new_instruction("loadI", str_size, NULL, "rbss", NULL);
  concat_inst(inst_aux2, inst_aux1);
  // Adiciona jump para a função main 
  inst_aux2 = _new_instruction("jumpI", NULL, NULL, label_main, NULL);
  concat_inst(inst_aux1, inst_aux2);
  concat_inst(inst_aux2, last_inst);

  return _extract_code(last_inst);
}

// Geração de código de identificadores
void gen_code_id(PROD_VALUE *id) {
  id->location = _new_register();

  char str_offset[12];
  sprintf(str_offset, "%d", id->table_entry.offset);

  switch (id->table_entry.entry_type) {
    case ET_VARIABLE:
      if (id->table_entry.global)
        id->code = _new_instruction("loadAI", "rbss", str_offset, id->location, NULL);
      else
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

// Geração de código para declaração de função
// Registro de ativaçao
  // 0 -> Endereço de retorno
  // 4 -> RSP salvo
  // 8 -> RFP salvo
  // 12 -> Início das variáveis locais
char *gen_code_function_def(PROD_VALUE *func, PROD_VALUE *cmd_list) {
  INSTRUCTION *aux_inst1, *aux_inst2;
  char *label = _new_label();
  func->location = NULL;
  // Cria um novo registro de ativação
  char str_offset[12];
  sprintf(str_offset, "%d", func->table_entry.offset);

  // Atualiza o RFP
  aux_inst1 = _new_instruction("i2i", "rsp", NULL, "rfp", label);
  // Atualiza o RSP
  aux_inst2 = _new_instruction("addI", "rsp", str_offset, "rsp", NULL);
  concat_inst(aux_inst1, aux_inst2);

  concat_inst(aux_inst2, cmd_list->code);
  func->code = cmd_list->code;

  return label;
}

// Geração de código de chamada de funçao
// Registro de ativaçao
  // 0 -> Endereço de retorno
  // 4 -> RSP salvo
  // 8 -> RFP salvo
  // 12 -> Início das variáveis locais
void gen_code_func_call(PROD_VALUE *func) {
  // Registrador de retorno da funçao
  func->location = _new_register();

  ENTRY_LIST *arg = func->list;
  INSTRUCTION *aux_inst1, *aux_inst2;
  int arg_offset = 12; 
  char str_offset[12];

  // Carrega os parâmetros para o registro de ativaçao
  while (arg != NULL) {
    sprintf(str_offset, "%d", arg_offset);
    aux_inst2 = _new_instruction("storeAI", arg->location, "rsp", str_offset, NULL);
    concat_inst(arg->code, aux_inst2);
    func->code = aux_inst2;
    arg_offset += arg->entry.size;
    arg = arg->next;
  }

  // Calcula o endereço de retorno (5 instruções abaixo)
  aux_inst1 = _new_instruction("addI", "rpc", "5", func->location, NULL);
  concat_inst(func->code, aux_inst1);
  // Salva o endereço de retorno
  aux_inst2 = _new_instruction("storeAI", func->location, "rsp", "0", NULL);
  concat_inst(aux_inst1, aux_inst2);
  // Salva o RSP
  aux_inst1 = _new_instruction("storeAI", "rsp", "rsp", "4", NULL);
  concat_inst(aux_inst2, aux_inst1);
  // Salva o RFP
  aux_inst2 = _new_instruction("storeAI", "rfp", "rsp", "8", NULL);
  concat_inst(aux_inst1, aux_inst2);
  // Transfere o controle para a funçao chamada
  aux_inst1 = _new_instruction("jumpI", NULL, NULL, func->table_entry.func_label, NULL);
  concat_inst(aux_inst2, aux_inst1);
  // Carrega o valor de retorno
  aux_inst2 = _new_instruction("loadAI", "rsp", "0", func->location, NULL);
  concat_inst(aux_inst1, aux_inst2);

  func->code = aux_inst2;
}

// Geração de código de retorno de funçao
// Registro de ativaçao
  // 0 -> Endereço de retorno
  // 4 -> RSP salvo
  // 8 -> RFP salvo
  // 12 -> Início das variáveis locais
void gen_code_func_return(PROD_VALUE *func, PROD_VALUE *val, int is_main) {
  INSTRUCTION *aux_inst1, *aux_inst2;

  if (!is_main) {
    char *reg_return = _new_register();
    char *reg_rsp = _new_register();
    char *reg_rfp = _new_register();

    // Salva o endereço de retorno
    aux_inst1 = _new_instruction("loadAI", "rfp", "0", reg_return, NULL);
    // Salva o rsp
    aux_inst2 = _new_instruction("loadAI", "rfp", "4", reg_rsp, NULL);
    concat_inst(aux_inst1, aux_inst2);
    // Salva o rfp
    aux_inst1 = _new_instruction("loadAI", "rfp", "8", reg_rfp, NULL);
    concat_inst(aux_inst2, aux_inst1);
    // Armazena o valor de retorno na base da pilha
    aux_inst2 = _new_instruction("storeAI", val->location, "rfp", "0", NULL);
    concat_inst(val->code, aux_inst2);
    concat_inst(aux_inst1, aux_inst2);
    // Atualiza o rsp
    aux_inst1 = _new_instruction("i2i", reg_rsp, NULL, "rsp", NULL);
    concat_inst(aux_inst2, aux_inst1);
    // Atualiza o rfp
    aux_inst2 = _new_instruction("i2i", reg_rfp, NULL, "rfp", NULL);
    concat_inst(aux_inst1, aux_inst2);
    // Devolve o controle ao chamador
    aux_inst1 = _new_instruction("jump", NULL, NULL, reg_return, NULL);
    concat_inst(aux_inst2, aux_inst1);
  } else  // Caso seja o retorna da main, insere a instruçao de HALT
    aux_inst1 = _new_instruction("halt", NULL, NULL, NULL, NULL);

  func->code = aux_inst1;
}


void _patch_holes(PATCH_LIST *list, char *label) {
  PATCH_LIST *hole = list, *temp;
  
  while (hole != NULL) {
    *(hole->label) = label;
    temp = hole->next;
    //free(hole);
    hole = temp;
  };
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

  if (list1 == NULL)
    return list2;
  else {
    while (last->next != NULL)
      last = last->next;
    last->next = list2;
    return list1;
  }
}

INSTRUCTION *_patch_expression(PATCH_LIST *tl, PATCH_LIST *fl, char* location) {
  INSTRUCTION *inst1=NULL, *inst2=NULL;
  char *label_end = _new_label();
  if (tl != NULL) {
    char *label_true = _new_label();
    _patch_holes(tl, label_true);

    inst1 = _new_instruction("loadI", "1", NULL, location, label_true);
    inst2 = _new_instruction("jumpI", NULL, NULL, label_end, NULL);
    concat_inst(inst1, inst2);
  }
  if (fl != NULL) {
    char *label_false = _new_label();
    _patch_holes(fl, label_false);
    
    inst1 = _new_instruction("loadI", "0", NULL, location, label_false);
    concat_inst(inst2, inst1);
    inst2 = _new_instruction("jumpI", NULL, NULL, label_end, NULL);
    concat_inst(inst1, inst2);
  }
  if (tl != NULL || fl != NULL) {
    inst1 = _new_instruction("nop", NULL, NULL, NULL, label_end);
    concat_inst(inst2, inst1);
  } else {
    return NULL;
  }

  return inst1;
}

// Geração de código de atribuição de variáveis
void gen_code_attribution(PROD_VALUE *var, PROD_VALUE *value) {
  INSTRUCTION *inst_aux = NULL;
  var->location = NULL;

  if (value->tl != NULL || value->fl != NULL)
    inst_aux = _patch_expression(value->tl, value->fl, value->location);

  if (value->location != NULL) {
    char str_offset[12];
    sprintf(str_offset, "%d", var->table_entry.offset);
    if (var->table_entry.global)
      var->code = _new_instruction("storeAI", value->location, "rbss", str_offset, NULL);
    else
      var->code = _new_instruction("storeAI", value->location, "rfp", str_offset, NULL);
  }
  if (inst_aux != NULL){
    concat_inst(value->code, inst_aux);
    concat_inst(inst_aux, var->code);
  }
  else {
    concat_inst(value->code, var->code);
  }
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

void _lit_to_logic(PROD_VALUE *lit) {
  if (lit->table_entry.entry_type != ET_LITERAL)
    return;
  
  switch(lit->table_entry.data_type) {
    case DT_INT:
      if (strcmp(lit->table_entry.key, "0")) {
        lit->code = _new_instruction("jumpI", NULL, NULL, "HOLE", NULL);
        lit->tl = _new_patch_list(&lit->code->arg3);
        lit->fl = NULL;
      } else {
        lit->code = _new_instruction("jumpI", NULL, NULL, "HOLE", NULL);
        lit->tl = NULL;
        lit->fl = _new_patch_list(&lit->code->arg3);
      }
      break;
  }
}

// Gera o código de operações lógicas (&& e ||)
void gen_code_logic_op(PROD_VALUE *exp, PROD_VALUE *op1, PROD_VALUE *operator, PROD_VALUE *op2) {
  INSTRUCTION *inst_aux;
  char *label = _new_label();
  exp->location = _new_register();

  if (op1->tl == NULL && op1->fl == NULL)
    _lit_to_logic(op1);
  if (op2->tl == NULL && op2->fl == NULL)
    _lit_to_logic(op2);

  if (!strcmp(operator->ast_node->label, "||")){
    _patch_holes(op1->fl, label);
    exp->code = _new_instruction("nop", NULL, NULL, NULL, label);
    exp->tl = _concat_patch_list(op1->tl, op2->tl);
    exp->fl = op2->fl;
  } else if (!strcmp(operator->ast_node->label, "&&")){
    _patch_holes(op1->tl, label);
    exp->code = _new_instruction("nop", NULL, NULL, NULL, label);
    exp->tl = op2->tl;
    exp->fl = _concat_patch_list(op1->fl, op2->fl);
  }
  concat_inst(op1->code, exp->code);
  concat_inst(exp->code, op2->code);

  exp->code = op2->code;
}

// Gera o código de expressões unárias (not)
void gen_code_unary_exp(PROD_VALUE *code, PROD_VALUE *op, PROD_VALUE *exp) {
  if (!strcmp(op->ast_node->label, "!")){
    code->location = exp->location;
    code->tl = exp->fl;
    code->fl = exp->tl;
    code->code = exp->code;
  }
}

// Gera o código do controlador de fluxo if
void gen_code_if(PROD_VALUE *code, PROD_VALUE *condition, PROD_VALUE *inst) {
  INSTRUCTION *inst_aux1=NULL, *inst_aux2=NULL;
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

// Gera o código do controlador de fluxo while
void gen_code_while(PROD_VALUE *code, PROD_VALUE *condition, PROD_VALUE *inst){
  INSTRUCTION *inst_aux1, *inst_aux2, *inst_aux3, *inst_aux4;
  code->location = _new_register();
  char *label_true = _new_label();
  char *label_false = _new_label();
  char *label_start = _new_label();

  _patch_holes(condition->tl, label_true);
  _patch_holes(condition->fl, label_false);

  inst_aux1 = _new_instruction("nop", NULL, NULL, NULL, label_start);
  concat_inst(inst_aux1, condition->code);
  inst_aux2 = _new_instruction("nop", NULL, NULL, NULL, label_true);
  concat_inst(condition->code, inst_aux2);
  concat_inst(inst_aux2, inst->code);
  inst_aux3 = _new_instruction("jumpI", NULL, NULL, label_start, NULL);
  concat_inst(inst->code, inst_aux3);
  inst_aux4 = _new_instruction("nop", NULL, NULL, NULL, label_false);
  concat_inst(inst_aux3, inst_aux4);
  code->code = inst_aux4;
}

// Gera o código do controlador de fluxo for
void gen_code_for(PROD_VALUE *code, PROD_VALUE *att1, PROD_VALUE *condition, PROD_VALUE *att2, PROD_VALUE *inst){
   INSTRUCTION *inst_aux1, *inst_aux2, *inst_aux3, *inst_aux4;
  code->location = _new_register();
  char *label_true = _new_label();
  char *label_false = _new_label();
  char *label_start = _new_label();

  _patch_holes(condition->tl, label_true);
  _patch_holes(condition->fl, label_false);
  
  inst_aux1 = _new_instruction("nop", NULL, NULL, NULL, label_start);
  concat_inst(att1->code, inst_aux1);
  concat_inst(inst_aux1, condition->code);
  inst_aux2 = _new_instruction("nop", NULL, NULL, NULL, label_true);
  concat_inst(condition->code, inst_aux2);
  concat_inst(inst_aux2, att2->code);
  concat_inst(att2->code, inst->code);
  inst_aux3 = _new_instruction("jumpI", NULL, NULL, label_start, NULL);
  concat_inst(inst->code, inst_aux3);
  inst_aux4 = _new_instruction("nop", NULL, NULL, NULL, label_false);
  concat_inst(inst_aux3, inst_aux4);
  code->code = inst_aux4;
}

// Geração de código para expressão Ternária
void gen_code_ternary(PROD_VALUE *code, PROD_VALUE *condition, PROD_VALUE *exp1, PROD_VALUE *exp2) {
  INSTRUCTION *inst_aux1, *inst_aux2, *inst_aux3, *inst_aux4, *inst_aux5;
  code->location = _new_register();
  char *label_true = _new_label();
  char *label_false = _new_label();
  char *label_end = _new_label();

  _patch_holes(condition->tl, label_true);
  _patch_holes(condition->fl, label_false);

  inst_aux1 = _new_instruction("loadI", "1", NULL, code->location, label_true);
  concat_inst(condition->code, inst_aux1);
  concat_inst(inst_aux1, exp1->code);
  inst_aux2 = _new_instruction("jumpI", NULL, NULL, label_end, NULL);
  concat_inst(exp1->code, inst_aux2);
  inst_aux3 = _new_instruction("loadI", "0", NULL, code->location, label_false);
  concat_inst(inst_aux2, inst_aux3);
  concat_inst(inst_aux3, exp2->code);
  inst_aux4 = _new_instruction("jumpI", NULL, NULL, label_end, NULL);
  concat_inst(exp2->code, inst_aux4);
  inst_aux5 = _new_instruction("nop", NULL, NULL, NULL, label_end);
  concat_inst(inst_aux4, inst_aux5);
  code->code = inst_aux5;
}