#include "errors.h"

extern STACK_ITEM *table_stack;

#define MAX_SHIFT_NUMBER 16

void throw_error(int err, SYMBOL_ENTRY entry) {
	print_err_msg(err, entry);
	exit(err);
}

void check_undeclared(SYMBOL_ENTRY symbol) {
  if (search_all_scopes(table_stack, symbol.key) == NULL)
    throw_error(ERR_UNDECLARED, symbol);
}

void check_declared(SYMBOL_ENTRY symbol) {
  if (search_local_scope(table_stack, symbol.key) != NULL)
    throw_error(ERR_DECLARED, symbol);
}

void check_variable(SYMBOL_ENTRY symbol) {
  SYMBOL_ENTRY *entry = search_all_scopes(table_stack, symbol.key);
  if (entry == NULL)
    throw_error(ERR_UNDECLARED, symbol);

  if (entry->entry_type != ET_VARIABLE) {
    if (entry->entry_type == ET_VECTOR)
      throw_error(ERR_VECTOR, symbol);
    if (entry->entry_type == ET_FUNCTION)
      throw_error(ERR_FUNCTION, symbol);
  }
}

void check_vector(SYMBOL_ENTRY symbol) {
  SYMBOL_ENTRY *entry = search_all_scopes(table_stack, symbol.key);
  if (entry == NULL)
    throw_error(ERR_UNDECLARED, symbol);

  if (entry->entry_type != ET_VECTOR) {
    if (entry->entry_type == ET_VARIABLE)
      throw_error(ERR_VARIABLE, symbol);
    if (entry->entry_type == ET_FUNCTION)
      throw_error(ERR_FUNCTION, symbol);
  }
}

void check_function(SYMBOL_ENTRY symbol) {
  SYMBOL_ENTRY *entry = search_all_scopes(table_stack, symbol.key);
  if (entry == NULL)
    throw_error(ERR_UNDECLARED, symbol);

  if (entry->entry_type != ET_FUNCTION) {
    if (entry->entry_type == ET_VARIABLE)
      throw_error(ERR_VARIABLE, symbol);
    if (entry->entry_type == ET_VECTOR)
      throw_error(ERR_VECTOR, symbol);
  }
}

void check_type(char *expected_label, SYMBOL_ENTRY symbol) {
  SYMBOL_ENTRY *entry1 = search_all_scopes(table_stack, expected_label);
  SYMBOL_ENTRY *entry2 = search_all_scopes(table_stack, symbol.key);

  if (entry1 == NULL)
    throw_error(ERR_UNDECLARED, *entry1);
  if (entry2 == NULL)
    throw_error(ERR_UNDECLARED, *entry2);

  switch (entry1->data_type) {
    case DT_INT:
    case DT_FLOAT:
    case DT_BOOL:
      if (entry2->data_type != DT_INT &&
          entry2->data_type != DT_FLOAT &&
          entry2->data_type != DT_BOOL)
        throw_error(ERR_WRONG_TYPE, *entry2);
      break;
    case DT_CHAR:
      if (entry2->data_type != DT_CHAR)
        throw_error(ERR_WRONG_TYPE, *entry2);
      break;
    case DT_STRING:
      if (entry2->data_type != DT_STRING)
        throw_error(ERR_WRONG_TYPE, *entry2);
      break;
  }
}

void check_input(SYMBOL_ENTRY symbol){
  SYMBOL_ENTRY *entry = search_all_scopes(table_stack, symbol.key);
  if (entry == NULL)
    throw_error(ERR_UNDECLARED, symbol);

  if (entry->data_type != DT_INT && entry->data_type != DT_FLOAT)
    throw_error(ERR_WRONG_PAR_INPUT, symbol);
}

void check_output(SYMBOL_ENTRY symbol){
  SYMBOL_ENTRY *entry = search_all_scopes(table_stack, symbol.key);
  if (entry == NULL)
    throw_error(ERR_UNDECLARED, symbol);

  if (entry->data_type != DT_INT && entry->data_type != DT_FLOAT)
    throw_error(ERR_WRONG_PAR_OUTPUT, symbol);
}

void check_shift(SYMBOL_ENTRY symbol, LEX_VALUE shift_value){
  SYMBOL_ENTRY *entry = search_all_scopes(table_stack, symbol.key);
  if (entry == NULL)
    throw_error(ERR_UNDECLARED, symbol);

    if (shift_value.value.i > MAX_SHIFT_NUMBER)
      throw_error(ERR_WRONG_PAR_SHIFT, symbol);
}

int infer_type(SYMBOL_ENTRY s1, SYMBOL_ENTRY s2) {
  if (s1.data_type == s2.data_type)
    return s1.data_type;
  if ((s1.data_type == DT_FLOAT && s2.data_type == DT_INT) ||
      (s1.data_type == DT_INT && s2.data_type == DT_FLOAT))
    return DT_FLOAT;
  if ((s1.data_type == DT_BOOL && s2.data_type == DT_INT) ||
      (s1.data_type == DT_INT && s2.data_type == DT_BOOL))
    return DT_INT;
  if ((s1.data_type == DT_BOOL && s2.data_type == DT_FLOAT) ||
      (s1.data_type == DT_FLOAT && s2.data_type == DT_BOOL))
    return DT_FLOAT;
  if (s1.data_type == DT_STRING && s2.data_type != DT_STRING)
    throw_error(ERR_STRING_TO_X, s1);
  if (s2.data_type == DT_STRING && s1.data_type != DT_STRING)
    throw_error(ERR_STRING_TO_X, s2);
  if (s1.data_type == DT_CHAR && s2.data_type != DT_CHAR)
    throw_error(ERR_CHAR_TO_X, s1);
  if (s2.data_type == DT_CHAR && s1.data_type != DT_CHAR)
    throw_error(ERR_CHAR_TO_X, s2);
}

void check_string_size(SYMBOL_ENTRY string1, int string2_size){
  if (string2_size > string1.size){
    throw_error(ERR_STRING_SIZE , string1);
  }
}

int check_is_string_op(char *label, int data_type_arg1, int data_type_arg2){
  if ( (!strcmp(label, "+")) && (data_type_arg1==DT_STRING) && (data_type_arg2==DT_STRING) ){
    return 1;
  }
  else
  {
    return 0;
  }

}

char *print_err_msg(int err, SYMBOL_ENTRY entry) {
  switch (err) {
    case ERR_UNDECLARED:
      printf("ERR_UNDECLARED: %s (line %d, column %d) is not declared.\n", entry.key, entry.line, entry.column);
      break;
    case ERR_DECLARED:
      printf("ERR_DECLARED: %s (line %d, column %d) was already declared.\n", entry.key, entry.line, entry.column);
      break;
    case ERR_VARIABLE:
      printf("ERR_VARIABLE: %s (line %d, column %d) is a VARIABLE, not a FUNCTION or a VECTOR.\n", entry.key, entry.line, entry.column);
      break;
    case ERR_VECTOR:
      printf("ERR_VECTOR: %s (line %d, column %d) is a VECTOR, not a FUNCTION or a VARIABLE.\n", entry.key, entry.line, entry.column);
      break;
    case ERR_FUNCTION:
      printf("ERR_FUNCTION: %s (line %d, column %d) is a FUNCTION, not a VARIABLE or a VECTOR.\n", entry.key, entry.line, entry.column);
      break;
    case ERR_WRONG_TYPE:
      printf("ERR_WRONG_TYPE: %s (line %d, column %d) has the wrong type.\n", entry.key, entry.line, entry.column);
      break;
    case ERR_STRING_TO_X:
      printf("ERR_STRING_TO_X: %s (line %d, column %d) is a STRING and cannot be cast.\n", entry.key, entry.line, entry.column);
      break;
    case ERR_CHAR_TO_X:
      printf("ERR_CHAR_TO_X: %s (line %d, column %d) is a CHAR and cannot be cast.\n", entry.key, entry.line, entry.column);
      break;
    case ERR_STRING_SIZE:
      printf("ERR_STRING_SIZE: String %s (line %d, column %d) has an incompatible size.\n", entry.key, entry.line, entry.column);
      break;
    case ERR_MISSING_ARGS:
      printf("ERR_MISSING_ARGS");
      break;
    case ERR_EXCESS_ARGS:
      printf("ERR_EXCESS_ARGS");
      break;
    case ERR_WRONG_TYPE_ARGS:
      printf("ERR_WRONG_TYPE_ARGS");
      break;
    case ERR_WRONG_PAR_INPUT:
      printf("ERR_WRONG_PAR_INPUT: %s (line %d, column %d) should be an INT or a FLOAT.\n", entry.key, entry.line, entry.column);
      break;
    case ERR_WRONG_PAR_OUTPUT:
      printf("ERR_WRONG_PAR_OUTPUT: %s (line %d, column %d) should be an INT or a FLOAT.\n", entry.key, entry.line, entry.column);
      break;
    case ERR_WRONG_PAR_RETURN:
      printf("ERR_WRONG_PAR_RETURN: %s (line %d, column %d) has the wrong return type.\n", entry.key, entry.line, entry.column);
      break;
    case ERR_WRONG_PAR_SHIFT:
      printf("ERR_WRONG_PAR_SHIFT: %s (line %d, column %d) não pode ser maior que 16.\n", entry.key, entry.line, entry.column);
      break;
    default:
      printf("UNDEFINED_ERROR");
  }
}