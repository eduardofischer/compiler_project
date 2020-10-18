#include "errors.h"

extern STACK_ITEM *table_stack;

#define MAX_SHIFT_NUMBER 16

void throw_error(int err, SYMBOL_ENTRY entry) {
	char *err_name = get_err_name(err);
	printf("%s: %s in line %d, column %d\n", err_name, entry.key, entry.line, entry.column);
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

  if (entry->entry_type != ET_VARIABLE)
    throw_error(ERR_VARIABLE, symbol);
}

void check_vector(SYMBOL_ENTRY symbol) {
  SYMBOL_ENTRY *entry = search_all_scopes(table_stack, symbol.key);
  if (entry == NULL)
    throw_error(ERR_UNDECLARED, symbol);

  if (entry->entry_type != ET_VECTOR)
    throw_error(ERR_VECTOR, symbol);
}

void check_function(SYMBOL_ENTRY symbol) {
  SYMBOL_ENTRY *entry = search_all_scopes(table_stack, symbol.key);
  if (entry == NULL)
    throw_error(ERR_UNDECLARED, symbol);

  if (entry->entry_type != ET_FUNCTION)
    throw_error(ERR_FUNCTION, symbol);
}

void check_type(char *expected_label, SYMBOL_ENTRY symbol) {
  SYMBOL_ENTRY *entry = search_all_scopes(table_stack, expected_label);
  if (entry == NULL)
    throw_error(ERR_UNDECLARED, symbol);

  switch (entry->data_type) {
    case DT_INT:
    case DT_FLOAT:
    case DT_BOOL:
      if (symbol.data_type != DT_INT &&
          symbol.data_type != DT_FLOAT &&
          symbol.data_type != DT_BOOL)
        throw_error(ERR_WRONG_TYPE, symbol);
      break;
    case DT_CHAR:
      if (symbol.data_type != DT_CHAR)
        throw_error(ERR_WRONG_TYPE, symbol);
      break;
    case DT_STRING:
      if (symbol.data_type != DT_STRING)
        throw_error(ERR_WRONG_TYPE, symbol);
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

char *get_err_name(int err) {
  switch (err) {
    case ERR_UNDECLARED:
      return "ERR_UNDECLARED";
    case ERR_DECLARED:
      return "ERR_DECLARED";
    case ERR_VARIABLE:
      return "ERR_VARIABLE";
    case ERR_VECTOR:
      return "ERR_VECTOR";
    case ERR_FUNCTION:
      return "ERR_FUNCTION";
    case ERR_WRONG_TYPE:
      return "ERR_WRONG_TYPE";
    case ERR_STRING_TO_X:
      return "ERR_STRING_TO_X";
    case ERR_CHAR_TO_X:
      return "ERR_CHAR_TO_X";
    case ERR_STRING_SIZE:
      return "ERR_STRING_SIZE";
    case ERR_MISSING_ARGS:
      return "ERR_MISSING_ARGS";
    case ERR_EXCESS_ARGS:
      return "ERR_EXCESS_ARGS";
    case ERR_WRONG_TYPE_ARGS:
      return "ERR_WRONG_TYPE_ARGS";
    case ERR_WRONG_PAR_INPUT:
      return "ERR_WRONG_PAR_INPUT";
    case ERR_WRONG_PAR_OUTPUT:
      return "ERR_WRONG_PAR_OUTPUT";
    case ERR_WRONG_PAR_RETURN:
      return "ERR_WRONG_PAR_RETURN";
    case ERR_WRONG_PAR_SHIFT:
      return "ERR_WRONG_PAR_SHIFT";
    default:
      return "UNDEFINED_ERROR";
  }
}