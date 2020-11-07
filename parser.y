%{
#include <stdio.h>
#include <stdlib.h>
#include "aux/types.h"
#include "aux/ast.h"
#include "aux/hashtable.h"
#include "aux/stack.h"
#include "aux/errors.h"
#include "aux/iloc_code_gen.h"

extern int line_number, column;

int yylex(void);
void yyerror (char const *s);
void process_binary_exp(PROD_VALUE *exp, PROD_VALUE *op1, PROD_VALUE *operator, PROD_VALUE *op2);

extern void *arvore;
extern STACK_ITEM *table_stack;
%}

// Habilita o output verboso
%define parse.error verbose

%union {
	PROD_VALUE node;
	LEX_VALUE valor_lexico;
}

%token <node> TK_PR_INT
%token <node> TK_PR_FLOAT
%token <node> TK_PR_BOOL
%token <node> TK_PR_CHAR
%token <node> TK_PR_STRING
%token TK_PR_IF
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_PR_CONST
%token TK_PR_STATIC
%token TK_PR_FOR
%token TK_PR_BREAK
%token TK_PR_CONTINUE
%token <valor_lexico> TK_OC_LE
%token <valor_lexico> TK_OC_GE
%token <valor_lexico> TK_OC_EQ
%token <valor_lexico> TK_OC_NE
%token <valor_lexico> TK_OC_AND
%token <valor_lexico> TK_OC_OR
%token <valor_lexico> TK_OC_SL
%token <valor_lexico> TK_OC_SR
%token <valor_lexico> TK_LIT_INT
%token <valor_lexico> TK_LIT_FLOAT
%token <valor_lexico> TK_LIT_FALSE
%token <valor_lexico> TK_LIT_TRUE
%token <valor_lexico> TK_LIT_CHAR
%token <valor_lexico> TK_LIT_STRING
%token <valor_lexico> TK_IDENTIFICADOR
%token <valor_lexico> TOKEN_ERRO

// Declaração de tipos
%type <node> program
%type <node> literal
%type <node> type
%type <node> id
%type <node> function_def_start
%type <node> function_def
%type <node> cmd_block
%type <node> command_list
%type <node> command
%type <node> local_var_decl
%type <node> local_var_init
%type <node> local_list
%type <node> var_attribution
%type <node> vector_index
%type <node> expression
%type <node> unary_op
%type <node> input
%type <node> output
%type <node> function_call
%type <node> arguments_list
%type <node> shift_left
%type <node> shift_right
%type <node> return
%type <node> break 
%type <node> continue 
%type <node> conditional_if_else 
%type <node> iterative_for_while 
%type <node> global_var_decl
%type <node> global_list
%type <node> parameter
%type <node> parameters_list

// ## Associatividade e precedência dos operadores

// Virgula
%left ','
// Atribuição
%right '='
// Operador Condicional
%right '?' ':'
// Operadores logicos
%left TK_OC_OR
%left TK_OC_AND
// Operadores Bitwise
%left '|'
%left '&'
// Operadores relacionais
%left TK_OC_EQ TK_OC_NE
%left TK_OC_GE '>'
%left TK_OC_LE '<'
// Bitwise shift esquerda e direita (<< e >>)
%left TK_OC_SL TK_OC_SR 
// Soma e subtração
%left '+' '-'
// Multiplicação, divisão, resto e exponenciação
%left '*' '/' '%' '^'

// Expressão com operador ternário
%right TERNARY
// Expressões com operador unário
%right UNARY

%start root

%%
root: program	 {
		arvore = (void*) $1.ast_node;
		printf("%s\n", extract_code($1.code));
	} 
	;
	
program: global_var_decl program {
		$$ = $1;
		$$.ast_node = $2.ast_node;
		if ($$.code != NULL)
			concat_inst($2.code, $$.code);
		else
			$$.code = $2.code;
	}
	| function_def program	{ $$ = $1; add_child($$.ast_node, $2.ast_node); }
	| %empty { $$.ast_node = NULL; }
	;

type: TK_PR_INT { $$.table_entry.data_type = DT_INT; }
	| TK_PR_FLOAT { $$.table_entry.data_type = DT_FLOAT; }
	| TK_PR_BOOL { $$.table_entry.data_type = DT_BOOL; }
	| TK_PR_CHAR { $$.table_entry.data_type = DT_CHAR; }
	| TK_PR_STRING { $$.table_entry.data_type = DT_STRING; }
	;

literal: TK_LIT_INT {
		$$.ast_node = create_node_lex_value($1);
		$$.table_entry = init_table_entry($1, $$.ast_node->label, ET_LITERAL, DT_INT);
	}
	| TK_LIT_FLOAT {
		$$.ast_node = create_node_lex_value($1);
		$$.table_entry = init_table_entry($1, $$.ast_node->label, ET_LITERAL, DT_FLOAT);
	}
	| TK_LIT_FALSE {
		$$.ast_node = create_node_lex_value($1);
		$$.table_entry = init_table_entry($1, $$.ast_node->label, ET_LITERAL, DT_BOOL);
	}
	| TK_LIT_TRUE {
		$$.ast_node = create_node_lex_value($1);
		$$.table_entry = init_table_entry($1, $$.ast_node->label, ET_LITERAL, DT_BOOL);
	}
	| TK_LIT_CHAR {
		$$.ast_node = create_node_lex_value($1);
		$$.table_entry = init_table_entry($1, $$.ast_node->label, ET_LITERAL, DT_CHAR);
	}
	| TK_LIT_STRING {
		$$.ast_node = create_node_lex_value($1);
		$$.table_entry = init_table_entry($1, $$.ast_node->label, ET_LITERAL, DT_STRING);
		$$.table_entry.size = assign_size_var_init($$.table_entry.data_type, $$.ast_node->valor_lexico->value);
	}
	;

// Declaração de variáveis globais
global_var_decl: TK_PR_STATIC type id global_list ';' { 
		$3.table_entry.entry_type = ET_VARIABLE;
		$3.table_entry.data_type = $2.table_entry.data_type;
		$3.table_entry.size = assign_size($2.table_entry.data_type);
		check_declared($3.table_entry);	
		insert_ht_entry(table_stack, $3.table_entry);
		libera($3.ast_node);
		
		// Adiciona o resto da lista de declarações na tabela de símbolos
		ENTRY_LIST *item = $4.list, *next_item;
		while (item != NULL) {
			item->entry.data_type = $2.table_entry.data_type;
			item->entry.size = assign_size($2.table_entry.data_type);
			insert_ht_entry(table_stack, item->entry);
			next_item = item->next;
			free(item);
			item = next_item;
		}
	}
	| type id global_list ';'  { 
		$2.table_entry.entry_type = ET_VARIABLE;
		$2.table_entry.data_type = $1.table_entry.data_type;
		$2.table_entry.size = assign_size($1.table_entry.data_type);
		check_declared($2.table_entry);
		insert_ht_entry(table_stack, $2.table_entry);
		libera($2.ast_node);
		
		// Adiciona o resto da lista de declarações na tabela de símbolos
		ENTRY_LIST *item = $3.list, *next_item;
		while (item != NULL) {
			item->entry.data_type = $1.table_entry.data_type;
			item->entry.size = assign_size($1.table_entry.data_type);
			insert_ht_entry(table_stack, item->entry);
			next_item = item->next;
			free(item);
			item = next_item;
		}
	}
	| type id '[' expression ']' global_list ';' { 
		$2.table_entry.entry_type = ET_VECTOR;
		$2.table_entry.data_type = $1.table_entry.data_type;
		$2.table_entry.size = assign_size_vector($1.table_entry.data_type, $4.ast_node->valor_lexico->value);
		check_declared($2.table_entry);
		insert_ht_entry(table_stack, $2.table_entry);
		libera($2.ast_node);
		libera($4.ast_node);
		
		// Adiciona o resto da lista de declarações na tabela de símbolos
		ENTRY_LIST *item = $6.list, *next_item;
		while (item != NULL) {
			item->entry.data_type = $1.table_entry.data_type;
			item->entry.size = assign_size($1.table_entry.data_type);
			insert_ht_entry(table_stack, item->entry);
			next_item = item->next;
			free(item);
			item = next_item;
		}
	}
	;
global_list: ',' id global_list {
		$2.table_entry.entry_type = ET_VARIABLE;
		check_declared($2.table_entry);
		// Preenche a lista de argumentos
		$$.list = malloc(sizeof(ENTRY_LIST));
		$$.list->entry = $2.table_entry;
		$$.list->next = $3.list;
		libera($2.ast_node);
	}
	| ',' id '[' expression ']' global_list {
		$2.table_entry.entry_type = ET_VECTOR;
		check_declared($2.table_entry);
		// Preenche a lista de argumentos
		$$.list = malloc(sizeof(ENTRY_LIST));
		$$.list->entry = $2.table_entry;
		$$.list->next = $6.list;
		libera($2.ast_node);
	}
	| %empty { $$.list = NULL; }
	;

vector_index: id '[' expression ']' {
		$$.ast_node = create_node("[]");
		add_child($$.ast_node, $1.ast_node);
		add_child($$.ast_node, $3.ast_node);

		check_vector($1.table_entry);
		$$.table_entry = $1.table_entry;
		$$.table_entry.entry_type = ET_VARIABLE;
		$$.table_entry.data_type = search_all_scopes(table_stack, $1.ast_node->label)->data_type;
	}

id: TK_IDENTIFICADOR {
		$$.ast_node = create_node_lex_value($1);
		$$.table_entry = init_table_entry($1, $$.ast_node->label, NOT_DEFINED, NOT_DEFINED);
	}

function_def: function_def_start command_list cmd_block_end {
		$$ = $1;
		$$.location = NULL;
		$$.code = $2.code;
		add_child($$.ast_node, $2.ast_node);
		check_return($1.table_entry, $2.table_entry.data_type);
		insert_ht_entry(table_stack, $$.table_entry);
	}
function_def_start: type id '(' parameter parameters_list ')' cmd_block_start { 
		$$ = $2;
		$$.table_entry.entry_type = ET_FUNCTION;
		$$.table_entry.data_type = $1.table_entry.data_type;
		$$.table_entry.arguments = $4.arg_list;
		$$.table_entry.arguments->next = $5.arg_list;
		// Injeta os argumentos da função no seu escopo
		check_declared($2.table_entry);
		inject_arguments(table_stack, $$.table_entry.arguments);
	}
	| type id '(' ')' cmd_block_start { 
		$$ = $2;
		$$.table_entry.entry_type = ET_FUNCTION;
		$$.table_entry.data_type = $1.table_entry.data_type;
		$$.table_entry.arguments = NULL;
		check_declared($2.table_entry);
	}
	| TK_PR_STATIC type id '(' parameter parameters_list ')' cmd_block_start { 
		$$ = $3;
		$$.table_entry.entry_type = ET_FUNCTION;
		$$.table_entry.data_type = $2.table_entry.data_type;
		$$.table_entry.arguments = $5.arg_list;
		$$.table_entry.arguments->next = $6.arg_list;
		// Injeta os argumentos da função no seu escopo
		check_declared($3.table_entry);
		inject_arguments(table_stack, $$.table_entry.arguments);
	}
	| TK_PR_STATIC type id '(' ')' cmd_block_start { 
		$$ = $3;
		$$.table_entry.entry_type = ET_FUNCTION;
		$$.table_entry.data_type = $2.table_entry.data_type;
		$$.table_entry.arguments = NULL;
		check_declared($3.table_entry);
	}
parameters_list: ',' parameter parameters_list {
		$$.arg_list = $2.arg_list;
		$$.arg_list->next = $3.arg_list;
	}
	| %empty { $$.arg_list = NULL; }
	;
parameter: type id {
		$$.arg_list = malloc(sizeof(ARG_LIST));
		$$.arg_list->id = strdup($2.table_entry.key);
		$$.arg_list->type = $1.table_entry.data_type;
		libera($2.ast_node); 	
	}
	| TK_PR_CONST type id {
		$$.arg_list = malloc(sizeof(ARG_LIST));
		$$.arg_list->id = strdup($3.table_entry.key);
		$$.arg_list->type = $2.table_entry.data_type;
		libera($3.ast_node);
	}

// Definição dos blocos de comandos
cmd_block_start: '{' { table_stack = new_scope(table_stack); }
cmd_block_end: '}' { table_stack = destroy_scope(table_stack); }
cmd_block: cmd_block_start command_list cmd_block_end { $$ = $2; }
command_list: command command_list {
		if ($1.ast_node != NULL) {
			$$.ast_node = $1.ast_node;
			add_child($$.ast_node, $2.ast_node);
		} else {
			$$.ast_node = $2.ast_node;
		}

		if ($2.code != NULL) {
			concat_inst($1.code, $2.code);
			$$.code = $2.code;
		} else {
			$$.code = $1.code;
		}

		if ($1.table_entry.entry_type == ET_RETURN) {
			$$.table_entry.entry_type = ET_RETURN;
			$$.table_entry.data_type = $1.table_entry.data_type;
		}
		if ($2.table_entry.entry_type == ET_RETURN) {
			$$.table_entry.entry_type = ET_RETURN;
			$$.table_entry.data_type = $2.table_entry.data_type;
		}
	}
	| %empty { $$.ast_node = NULL; }
	;
command: local_var_decl ';' { $$ = $1; }
	| local_var_init ';'  { $$ = $1; }
	| var_attribution ';' { $$ = $1; }
	| function_call ';' { $$ = $1; }
	| input ';' { $$ = $1; }
	| output ';' { $$ = $1; }
	| shift_left ';' { $$ = $1; }
	| shift_right ';' { $$ = $1; }
	| return ';' {
		$$ = $1;
		$$.table_entry.entry_type = ET_RETURN;
	}
	| break';' { $$ = $1; }
	| continue ';' { $$ = $1; }
	| conditional_if_else ';' { $$ = $1; }
	| iterative_for_while ';' { $$ = $1; }
	| cmd_block ';' { $$ = $1; }
	;
	
// Declaração de variaveis locais
local_var_decl: local_var_prefix type id local_list { 
		$$ = $4; 
		$3.table_entry.entry_type = ET_VARIABLE;
		$3.table_entry.data_type = $2.table_entry.data_type;
		$3.table_entry.size = assign_size($2.table_entry.data_type);
		check_declared($3.table_entry);
		insert_ht_entry(table_stack, $3.table_entry);	
		libera($3.ast_node); 	
		// Adiciona o resto da lista de declarações na tabela de símbolos
		ENTRY_LIST *item = $4.list, *next_item;
		while (item != NULL) {
			item->entry.data_type = $2.table_entry.data_type;
			item->entry.size = assign_size($2.table_entry.data_type);
			insert_ht_entry(table_stack, item->entry);
			next_item = item->next;
			free(item);
			item = next_item;
		}
}

local_var_init: local_var_prefix type id TK_OC_LE id local_list {
		$$.ast_node = create_node_lex_value($4);
		add_child($$.ast_node, $3.ast_node);
		add_child($$.ast_node, $5.ast_node);
		add_child($$.ast_node, $6.ast_node);
		
		$3.table_entry.entry_type = ET_VARIABLE;
		$3.table_entry.data_type = $2.table_entry.data_type;
		$3.table_entry.size = assign_size_var_init($2.table_entry.data_type, $5.ast_node->valor_lexico->value);
		check_declared($3.table_entry);
		check_variable($5.table_entry);
		insert_ht_entry(table_stack, $3.table_entry);
		check_type($3.table_entry, $5.table_entry);
		// Adiciona o resto da lista de declarações na tabela de símbolos
		ENTRY_LIST *item = $6.list, *next_item;
		while (item != NULL) {
			item->entry.data_type = $2.table_entry.data_type;
			item->entry.size = assign_size($2.table_entry.data_type);
			insert_ht_entry(table_stack, item->entry);
			next_item = item->next;
			free(item);
			item = next_item;
		}
	}
	| local_var_prefix type id TK_OC_LE literal local_list {
		$$.ast_node = create_node_lex_value($4);
		add_child($$.ast_node, $3.ast_node);
		add_child($$.ast_node, $5.ast_node);
		add_child($$.ast_node, $6.ast_node);
		
		$3.table_entry.entry_type = ET_VARIABLE;
		$3.table_entry.data_type = $2.table_entry.data_type;
		$3.table_entry.size = assign_size_var_init($2.table_entry.data_type, $5.ast_node->valor_lexico->value);
		check_declared($3.table_entry);
		insert_ht_entry(table_stack, $3.table_entry);	
		check_type($3.table_entry, $5.table_entry);
		// Adiciona o resto da lista de declarações na tabela de símbolos
		ENTRY_LIST *item = $6.list, *next_item;
		while (item != NULL) {
			item->entry.data_type = $2.table_entry.data_type;
			item->entry.size = assign_size($2.table_entry.data_type);
			insert_ht_entry(table_stack, item->entry);
			next_item = item->next;
			free(item);
			item = next_item;
		}
	}

local_var_prefix: TK_PR_STATIC
	| TK_PR_CONST
	| TK_PR_STATIC TK_PR_CONST
	| %empty
	;
local_list: ',' id local_list {
		$$.ast_node = NULL;
		check_declared($2.table_entry);
		// Preenche a lista de argumentos
		$$.list = malloc(sizeof(ENTRY_LIST));
		$$.list->entry = $2.table_entry;
		$$.list->next = $3.list;
		libera($2.ast_node); 
	}
	| ',' id TK_OC_LE id local_list {
		$$.ast_node = create_node_lex_value($3); 
		add_child($$.ast_node, $2.ast_node); 
		add_child($$.ast_node, $4.ast_node); 
		add_child($$.ast_node, $5.ast_node);
		check_declared($2.table_entry);
		check_variable($4.table_entry);
		// Preenche a lista de argumentos
		$$.list = malloc(sizeof(ENTRY_LIST));
		$$.list->entry = $2.table_entry;
		$$.list->next = $5.list;
	}
	| ',' id TK_OC_LE literal local_list { 
		$$.ast_node = create_node_lex_value($3); 
		add_child($$.ast_node, $2.ast_node); 
		add_child($$.ast_node, $4.ast_node); 
		add_child($$.ast_node, $5.ast_node); 
		check_declared($2.table_entry);
		// Preenche a lista de argumentos
		$$.list = malloc(sizeof(ENTRY_LIST));
		$$.list->entry = $2.table_entry;
		$$.list->next = $5.list;
	}
	| %empty {
		$$.ast_node = NULL;
		$$.list = NULL;
	}
	;

// Atribuição de variavel
var_attribution: id '=' expression {
		$$.ast_node = create_node("="); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, $3.ast_node); 

		check_type($1.table_entry, $3.table_entry);

		$1.table_entry = *find_table_entry(table_stack, $1.table_entry);
		if ($3.table_entry.entry_type != ET_LITERAL)
			$3.table_entry = *find_table_entry(table_stack, $3.table_entry);
			
		if($1.table_entry.data_type == DT_STRING)
			check_string_size($1.table_entry, $3.table_entry.size);
	
		$$.table_entry.offset= $1.table_entry.offset;
		gen_code_attribution(&$$, &$3);
	}
	| vector_index '=' expression { 
		$$.ast_node = create_node("="); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, $3.ast_node);

		check_type($1.table_entry, $3.table_entry);

		$1.table_entry.data_type = find_table_entry(table_stack, $1.table_entry)->data_type;
		$1.table_entry.size = find_table_entry(table_stack, $1.table_entry)->size;
		if($1.table_entry.data_type == DT_STRING)
			check_string_size($1.table_entry, $3.table_entry.size);
		free_entry($1.table_entry);
	}
	;
	
// Expressões da linguagem
expression: id {
		$$ = $1;
		$$.table_entry = *find_table_entry(table_stack, $1.table_entry);
		gen_code_id(&$$);
	}
	| vector_index { $$ = $1; }
	| function_call  { $$ = $1; }
	| literal {
		$$ = $1;
		gen_code_literal(&$$);
	}
	| '(' expression ')'  { $$ = $2; }
	| unary_op expression %prec UNARY {
		$$.ast_node = $1.ast_node; 
		add_child($$.ast_node, $2.ast_node);
		$$.table_entry = $2.table_entry;
	}
	// Expressões binárias
	| expression '+' expression {
		PROD_VALUE op;
		op.ast_node = create_node("+");
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression '-' expression {
		PROD_VALUE op;
		op.ast_node = create_node("-");
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression '*' expression {
		PROD_VALUE op;
		op.ast_node = create_node("*");
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression '/' expression	{
		PROD_VALUE op;
		op.ast_node = create_node("/");
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression '%' expression	{
		PROD_VALUE op;
		op.ast_node = create_node("%");
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression '^' expression	{
		PROD_VALUE op;
		op.ast_node = create_node("^");
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression '<' expression { 
		PROD_VALUE op;
		op.ast_node = create_node("<");
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
		
		char *str = "Remendo1";
		$$.list_f = malloc(sizeof(LIST));
		$$.list_f->rot = malloc(sizeof(char)+1);
		$$.list_f->rot= str;
		$$.list_f->next = NULL;
		//printf("%s\n", $$.list_f->rot);
	}
	| expression '>' expression { 
		PROD_VALUE op;
		op.ast_node = create_node(">");
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression '|' expression {
		PROD_VALUE op;
		op.ast_node = create_node("|");
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression '&' expression {
		PROD_VALUE op;
		op.ast_node = create_node("&");
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}													
	| expression TK_OC_LE expression { 
		PROD_VALUE op;
		op.ast_node = create_node_lex_value($2); 
		op.table_entry.data_type = DT_BOOL;
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression TK_OC_GE expression { 
		PROD_VALUE op;
		op.ast_node = create_node_lex_value($2);
		op.table_entry.data_type = DT_BOOL; 
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression TK_OC_EQ expression { 
		PROD_VALUE op;
		op.ast_node = create_node_lex_value($2);
		op.table_entry.data_type = DT_BOOL; 
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression TK_OC_NE expression { 
		PROD_VALUE op;
		op.ast_node = create_node_lex_value($2);
		op.table_entry.data_type = DT_BOOL; 
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression TK_OC_OR expression {
		PROD_VALUE op;
		op.ast_node = create_node_lex_value($2);
		op.table_entry.data_type = DT_BOOL; 
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression TK_OC_AND expression { 
		PROD_VALUE op;
		op.ast_node = create_node_lex_value($2);
		op.table_entry.data_type = DT_BOOL; 
		process_binary_exp(&$$, &$1, &op, &$3);
		gen_code_binary_exp(&$$, &$1, &op, &$3);
	}
	| expression '?' expression ':' expression %prec TERNARY { 
		$$.ast_node = create_node("?:"); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, $3.ast_node); 
		add_child($$.ast_node, $5.ast_node);
		$$.table_entry.data_type = infer_type($3.table_entry, $5.table_entry);
	}
	;
unary_op: '+' { $$.ast_node = create_node("+"); }
	| '-' { $$.ast_node = create_node("-"); }
	| '!' {
		$$.ast_node = create_node("!");
		$$.table_entry.data_type = DT_BOOL;
	}
	| '&' { $$.ast_node = create_node("&"); }
	| '*' { $$.ast_node = create_node("*"); }
	| '?' {
		$$.ast_node = create_node("?");
		$$.table_entry.data_type = DT_BOOL;
	}
	| '#' { $$.ast_node = create_node("#"); }
	;
// Comandos de entrada e saida
input: TK_PR_INPUT id { 
		$$.ast_node = create_node("input"); 
		add_child($$.ast_node, $2.ast_node);

		check_input($2.table_entry);
	}
	;
output: TK_PR_OUTPUT id { 
		$$.ast_node = create_node("output"); 
		add_child($$.ast_node, $2.ast_node); 

		check_output($2.table_entry);
	}
	| TK_PR_OUTPUT literal { 
		$$.ast_node = create_node("output"); 
		add_child($$.ast_node, $2.ast_node);

		check_output($2.table_entry); 
	}
	;
	
// Chamada de Função
function_call: id '(' expression arguments_list ')' { 
		$$.ast_node = create_node("call "); 
		concat_label(&($$.ast_node->label), $1.table_entry.key); 
		add_child($$.ast_node, $3.ast_node); 
		add_child($3.ast_node, $4.ast_node);

		$3.table_entry.data_type = find_table_entry(table_stack, $3.table_entry)->data_type;
		check_function($1.table_entry);
		// Preenche a lista de argumentos
		$$.list = malloc(sizeof(ENTRY_LIST));
		$$.list->entry = $3.table_entry;
		$$.list->next = $4.list;
		check_args($1.table_entry, $$.list);
		$$.table_entry.data_type = search_all_scopes(table_stack, $1.table_entry.key)->data_type;
		libera($1.ast_node);
	}
	| id '(' ')' {
		$$.table_entry = $1.table_entry;
		$$.ast_node = create_node("call "); 
		concat_label(&($$.ast_node->label), $1.table_entry.key);
		check_function($1.table_entry);
		check_args($1.table_entry, NULL);
		libera($1.ast_node);
	}
	| vector_index '(' expression arguments_list ')' { 
		$$.ast_node = create_node("call "); 
		// TODO: concat_label(&($$.ast_node->label), $1.ast_node->children[0]->label); 
		add_child($$.ast_node, $3.ast_node); 
		add_child($3.ast_node, $4.ast_node); 

		$3.table_entry.data_type = find_table_entry(table_stack, $3.table_entry)->data_type;
		// Preenche a lista de argumentos
		$$.list = malloc(sizeof(ENTRY_LIST));
		$$.list->entry = $3.table_entry;
		$$.list->next = $4.list;
		check_args($1.table_entry, $$.list);
		$$.table_entry.data_type = search_all_scopes(table_stack, $1.table_entry.key)->data_type;
		libera($1.ast_node);
	}
	| vector_index '(' ')' { 
		$$.ast_node = create_node("call "); 
		// TODO: concat_label(&($$.ast_node->label), $1.ast_node->children[0]->label); 
		check_args($1.table_entry, NULL);
		libera($1.ast_node); 
	}
	;
arguments_list: ',' expression arguments_list { 
		$$ = $2; 
		add_child($$.ast_node, $3.ast_node); 
		// Preenche a lista de argumentos
		$2.table_entry.data_type = find_table_entry(table_stack, $2.table_entry)->data_type;
		$$.list = malloc(sizeof(ENTRY_LIST));
		$$.list->entry = $2.table_entry;
		$$.list->next = $3.list;
	}
	| %empty {
		$$.ast_node = NULL;
		$$.list = NULL;
	}
	;

// Comandos de shift
shift_left: id TK_OC_SL TK_LIT_INT { 
		$$.ast_node = create_node_lex_value($2); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, create_node_lex_value($3)); 

		check_shift($1.table_entry, $3);
	}
	| vector_index TK_OC_SL TK_LIT_INT { 
		$$.ast_node = create_node_lex_value($2); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, create_node_lex_value($3)); 

		check_shift($1.table_entry, $3);
	}
	;	
shift_right: id TK_OC_SR TK_LIT_INT { 
		$$.ast_node = create_node_lex_value($2); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, create_node_lex_value($3)); 

		check_shift($1.table_entry, $3);
	}
	| vector_index TK_OC_SR TK_LIT_INT { 
		$$.ast_node = create_node_lex_value($2); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, create_node_lex_value($3)); 

		check_shift($1.table_entry, $3);
	}
	;

// Comandos return, break e continue
return: TK_PR_RETURN expression { 
		$$.ast_node = create_node("return"); 
		add_child($$.ast_node, $2.ast_node);
		if ($2.table_entry.entry_type != ET_LITERAL)
			$$.table_entry.data_type = find_table_entry(table_stack, $2.table_entry)->data_type;
		else
			$$.table_entry = $2.table_entry;
	}
break: TK_PR_BREAK { $$.ast_node = create_node("break"); }
continue: TK_PR_CONTINUE { $$.ast_node = create_node("continue"); }

// Comandos de controle de fluxo
conditional_if_else: TK_PR_IF '(' expression ')' cmd_block { 
		$$.ast_node = create_node("if"); 
		add_child($$.ast_node, $3.ast_node); 
		add_child($$.ast_node, $5.ast_node); 
	}
	| TK_PR_IF '(' expression ')' cmd_block TK_PR_ELSE cmd_block { 
		$$.ast_node = create_node("if"); 
		add_child($$.ast_node, $3.ast_node); 
		add_child($$.ast_node, $5.ast_node); 
		add_child($$.ast_node, $7.ast_node); 
	}
	;
iterative_for_while: TK_PR_FOR '(' var_attribution ':' expression ':' var_attribution ')' cmd_block { 
		$$.ast_node = create_node("for"); 
		add_child($$.ast_node, $3.ast_node); 
		add_child($$.ast_node, $5.ast_node); 
		add_child($$.ast_node, $7.ast_node); 
		add_child($$.ast_node, $9.ast_node); 
	}
	| TK_PR_WHILE '(' expression ')' TK_PR_DO cmd_block { 
		$$.ast_node = create_node("while"); 
		add_child($$.ast_node, $3.ast_node); 
		add_child($$.ast_node, $6.ast_node); 
	}

%%

void yyerror(char const *s) {
	printf("%s in line %d, column %d\n", s, line_number, column);
	exit(1);
}

void process_binary_exp(PROD_VALUE *exp, PROD_VALUE *op1, PROD_VALUE *operator, PROD_VALUE *op2) {
	exp->ast_node = operator->ast_node;
	add_child(exp->ast_node, op1->ast_node); 
	add_child(exp->ast_node, op2->ast_node);

	if (operator->table_entry.data_type > 0)
		exp->table_entry.data_type = operator->table_entry.data_type;
	else
		exp->table_entry.data_type = infer_type(op1->table_entry, op2->table_entry);

	if (op1->table_entry.entry_type != ET_LITERAL) {
    SYMBOL_ENTRY *entry1 = find_table_entry(table_stack, op1->table_entry);
		op1->table_entry.data_type = entry1->data_type;
		op1->table_entry.size = entry1->size;
  }
  if (op2->table_entry.entry_type != ET_LITERAL) {
    SYMBOL_ENTRY *entry2 = find_table_entry(table_stack, op2->table_entry);
		op2->table_entry.data_type = entry2->data_type;
		op2->table_entry.size = entry2->size;
  }
	
	if(check_is_string_op(operator->ast_node->label, op1->table_entry.data_type, op2->table_entry.data_type))
		exp->table_entry.size = op1->table_entry.size + op2->table_entry.size;
}
