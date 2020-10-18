#include "errors.h"

extern STACK_ITEM *table_stack;

void throw_error(int err, char *label, SYMBOL_ENTRY entry) {
	char *err_name = get_err_name(err);
	printf("%s: %s na linha %d, coluna %d\n", err_name, label, entry.line, entry.column);
	exit(err);
}

void check_undeclared(char *label, SYMBOL_ENTRY symbol) {
  STACK_ITEM *scope = table_stack;

  while (scope != NULL) {
    if (get_ht_entry(top(scope), label) != NULL)
      return;
    scope = scope->next;
  };

  throw_error(ERR_UNDECLARED, label , symbol);
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