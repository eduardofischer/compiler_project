%{
#include <stdio.h>
#include <stdlib.h>
#include "aux/types.h"
#include "aux/ast.h"
#include "aux/hashtable.h"
#include "aux/stack.h"

extern int line_number, column;

int yylex(void);
void yyerror (char const *s);

extern void *arvore;
extern STACK_ITEM *table_stack;
%}

// Habilita o output verboso
%define parse.error verbose

%union {
	PROD_VALUE node;
	LEX_VALUE valor_lexico;
	ARG_LIST *argument;
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
%type <node> binary_op
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
%type <argument> parameter
%type <argument> parameters_list

// ## Associatividade e prioridade dos operadores

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
// Expressões com operador binário
%left BINARY
// Expressões com operador unário
%right UNARY

%start root

%%
root: program	 { arvore = (void*) $1.ast_node;  } 
	;
	
program: global_var_decl program { $$.ast_node = NULL; }
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
		$$.table_entry = init_table_entry($1, ET_LITERAL, DT_INT);
		insert_ht_entry(top(table_stack), $$.ast_node->label, $$.table_entry);
	}
	| TK_LIT_FLOAT {
		$$.ast_node = create_node_lex_value($1);
		$$.table_entry = init_table_entry($1, ET_LITERAL, DT_FLOAT);
		insert_ht_entry(top(table_stack), $$.ast_node->label, $$.table_entry);	
	}
	| TK_LIT_FALSE {
		$$.ast_node = create_node_lex_value($1);
		$$.table_entry = init_table_entry($1, ET_LITERAL, DT_BOOL);
		insert_ht_entry(top(table_stack), $$.ast_node->label, $$.table_entry);
	}
	| TK_LIT_TRUE {
		$$.ast_node = create_node_lex_value($1);
		$$.table_entry = init_table_entry($1, ET_LITERAL, DT_BOOL);
		insert_ht_entry(top(table_stack), $$.ast_node->label, $$.table_entry);
	}
	| TK_LIT_CHAR {
		$$.ast_node = create_node_lex_value($1);
		$$.table_entry = init_table_entry($1, ET_LITERAL, DT_CHAR);
		insert_ht_entry(top(table_stack), $$.ast_node->label, $$.table_entry);
	}
	| TK_LIT_STRING {
		$$.ast_node = create_node_lex_value($1);
		$$.table_entry = init_table_entry($1, ET_LITERAL, DT_STRING);
		insert_ht_entry(top(table_stack), $$.ast_node->label, $$.table_entry);
	}
	;

// Declaração de variáveis globais
global_var_decl: TK_PR_STATIC type id global_list ';' { 
		$3.table_entry.entry_type = ET_VARIABLE;
		$3.table_entry.data_type = $2.table_entry.data_type;
		insert_ht_entry(top(table_stack), $3.ast_node->label, $3.table_entry);	
	}
	| type id global_list ';'  { 
		$2.table_entry.entry_type = ET_VARIABLE;
		$2.table_entry.data_type = $1.table_entry.data_type;
		insert_ht_entry(top(table_stack), $2.ast_node->label, $2.table_entry);	
	}
	| type vector_index global_list ';' { 
		$2.table_entry.entry_type = ET_VARIABLE;
		$2.table_entry.data_type = $1.table_entry.data_type;
		insert_ht_entry(top(table_stack), $2.ast_node->label, $2.table_entry);	
	}
	;
global_list: ',' id global_list
	| ',' vector_index global_list
	| %empty
	;

vector_index: id '[' expression ']' {
		$$.ast_node = create_node("[]");
		add_child($$.ast_node, $1.ast_node);
		add_child($$.ast_node, $3.ast_node);
	}

id: TK_IDENTIFICADOR {
		$$.ast_node = create_node_lex_value($1);

		$$.table_entry = init_table_entry($1, NOT_DEFINED, NOT_DEFINED);
	}

function_def: type id '(' parameter parameters_list ')' cmd_block { 
		$$ = $2; add_child($$.ast_node, $7.ast_node); 
		$2.table_entry.entry_type = ET_FUNCTION;
		$2.table_entry.data_type = $1.table_entry.data_type;

		$2.table_entry.arguments = $4;
		$4->next = $5;
		
		insert_ht_entry(top(table_stack), $2.ast_node->label, $2.table_entry);
	}
	| type id '(' ')' cmd_block { 
		$$ = $2; add_child($$.ast_node, $5.ast_node);
		$2.table_entry.entry_type = ET_FUNCTION;
		$2.table_entry.data_type = $1.table_entry.data_type;
		insert_ht_entry(top(table_stack), $2.ast_node->label, $2.table_entry);
	}
	| TK_PR_STATIC type id '(' parameter parameters_list ')' cmd_block { 
		$$ = $3; add_child($$.ast_node, $8.ast_node); 
		$3.table_entry.entry_type = ET_FUNCTION;
		$3.table_entry.data_type = $2.table_entry.data_type;
		insert_ht_entry(top(table_stack), $3.ast_node->label, $3.table_entry);		
	}
	| TK_PR_STATIC type id '(' ')' cmd_block { 
		$$ = $3; add_child($$.ast_node, $6.ast_node); 
		$3.table_entry.entry_type = ET_FUNCTION;
		$3.table_entry.data_type = $2.table_entry.data_type;
		insert_ht_entry(top(table_stack), $2.ast_node->label, $2.table_entry); 
	}
	;
parameters_list: ',' parameter parameters_list {
		$$ = $2;
		$$->next = $3;
	}
	| %empty { $$ = NULL; }
	;
parameter: type id {
		$2.table_entry.entry_type = ET_VARIABLE;
		$2.table_entry.data_type = $1.table_entry.data_type;
		insert_ht_entry(top(table_stack), $2.ast_node->label, $2.table_entry);

		$$ = malloc(sizeof(ARG_LIST));
		$$->id = $2.ast_node->label;
		$$->type = $1.table_entry.data_type;
		
		libera($2.ast_node); 	
	}
	| TK_PR_CONST type id {
		$3.table_entry.entry_type = ET_VARIABLE;
		$3.table_entry.data_type = $2.table_entry.data_type;
		insert_ht_entry(top(table_stack), $3.ast_node->label, $3.table_entry);
		 
		libera($3.ast_node);
	}
	;

// Definição dos blocos de comandos
cmd_block_start: '{' { table_stack = new_scope(table_stack); }
cmd_block_end: '}' { table_stack = pop(table_stack); }
cmd_block: cmd_block_start command_list cmd_block_end { $$ = $2; }
command_list: command command_list {
		if ($1.ast_node != NULL) {
			$$ = $1;
			add_child($$.ast_node, $2.ast_node);
		} else {
			$$.ast_node = $2.ast_node;
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
	| return ';' { $$ = $1; }
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
		insert_ht_entry(top(table_stack), $3.ast_node->label, $3.table_entry);	
		
		libera($3.ast_node); 	
}
	;
local_var_init: local_var_prefix type id TK_OC_LE id local_list {
		$$.ast_node = create_node_lex_value($4);
		add_child($$.ast_node, $3.ast_node);
		add_child($$.ast_node, $5.ast_node);
		add_child($$.ast_node, $6.ast_node);
		
		$3.table_entry.entry_type = ET_VARIABLE;
		$3.table_entry.data_type = $2.table_entry.data_type;
		insert_ht_entry(top(table_stack), $3.ast_node->label, $3.table_entry);
		
		$5.table_entry.entry_type = ET_VARIABLE;
		$5.table_entry.data_type = $2.table_entry.data_type;
		insert_ht_entry(top(table_stack), $5.ast_node->label, $5.table_entry);
	}
	| local_var_prefix type id TK_OC_LE literal local_list {
		$$.ast_node = create_node_lex_value($4);
		add_child($$.ast_node, $3.ast_node);
		add_child($$.ast_node, $5.ast_node);
		add_child($$.ast_node, $6.ast_node);
		
		$3.table_entry.entry_type = ET_VARIABLE;
		$3.table_entry.data_type = $2.table_entry.data_type;
		insert_ht_entry(top(table_stack), $3.ast_node->label, $3.table_entry);
	}
	;
local_var_prefix: TK_PR_STATIC
	| TK_PR_CONST
	| TK_PR_STATIC TK_PR_CONST
	| %empty
	;
local_list: ',' id local_list { 
		$$.ast_node = NULL;
		libera($2.ast_node); 

}
	| ',' id TK_OC_LE id local_list { 
		$$.ast_node = create_node_lex_value($3); 
		add_child($$.ast_node, $2.ast_node); 
		add_child($$.ast_node, $4.ast_node); 
		add_child($$.ast_node, $5.ast_node); 
	}
	| ',' id TK_OC_LE literal local_list { 
		$$.ast_node = create_node_lex_value($3); 
		add_child($$.ast_node, $2.ast_node); 
		add_child($$.ast_node, $4.ast_node); 
		add_child($$.ast_node, $5.ast_node); 
	}
	| %empty { $$.ast_node = NULL; }
	;

// Atribuição de variavel
var_attribution: id '=' expression { 
		$$.ast_node = create_node("="); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, $3.ast_node); 
	}
	| vector_index '=' expression { 
		$$.ast_node = create_node("="); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, $3.ast_node); 
	}
	;
	
// Expressões da linguagem
expression: id { $$ = $1; }
	| vector_index { $$ = $1; }
	| function_call  { $$ = $1; }
	| literal { $$ = $1; }
	| '(' expression ')'  { $$ = $2; }
	| unary_op expression %prec UNARY {
		$$ = $1; 
		add_child($$.ast_node, $2.ast_node); 
	}
	| expression binary_op expression %prec BINARY { 
		$$ = $2; 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, $3.ast_node); 
	}
	| expression '?' expression ':' expression %prec TERNARY { 
		$$.ast_node = create_node("?:"); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, $3.ast_node); 
		add_child($$.ast_node, $5.ast_node);
	}
	;
unary_op: '+' { $$.ast_node = create_node("+"); }
	| '-' { $$.ast_node = create_node("-"); }
	| '!' { $$.ast_node = create_node("!"); }
	| '&' { $$.ast_node = create_node("&"); }
	| '*' { $$.ast_node = create_node("*"); }
	| '?' { $$.ast_node = create_node("?"); }
	| '#' { $$.ast_node = create_node("#"); }
	;
binary_op: '+' { $$.ast_node = create_node("+"); }
	| '-' { $$.ast_node = create_node("-"); }
	| '*' { $$.ast_node = create_node("*"); }
	| '/'	{ $$.ast_node = create_node("/"); }
	| '%'	{ $$.ast_node = create_node("%"); }
	| '^'	{ $$.ast_node = create_node("^"); }
	| '<' { $$.ast_node = create_node("<"); }
	| '>' { $$.ast_node = create_node(">"); }
	| '|' { $$.ast_node = create_node("|"); }
	| '&' { $$.ast_node = create_node("&"); }													
	| TK_OC_LE { $$.ast_node = create_node_lex_value($1); }
	| TK_OC_GE { $$.ast_node = create_node_lex_value($1); }
	| TK_OC_EQ { $$.ast_node = create_node_lex_value($1); }
	| TK_OC_NE { $$.ast_node = create_node_lex_value($1); }
	| TK_OC_OR { $$.ast_node = create_node_lex_value($1); }
	| TK_OC_AND { $$.ast_node = create_node_lex_value($1); }
	;

// Comandos de entrada e saida
input: TK_PR_INPUT id { 
		$$.ast_node = create_node("input"); 
		add_child($$.ast_node, $2.ast_node); 
	}
	;
output: TK_PR_OUTPUT id { 
		$$.ast_node = create_node("output"); 
		add_child($$.ast_node, $2.ast_node); 
	}
	| TK_PR_OUTPUT literal { 
		$$.ast_node = create_node("output"); 
		add_child($$.ast_node, $2.ast_node); 
	}
	;
	
// Chamada de Função
function_call: id '(' expression arguments_list ')' { 
		$$.ast_node = create_node("call "); 
		// TODO: concat_label(&($$.ast_node->label), $1.table_entry.value.s); 
		add_child($$.ast_node, $3.ast_node); 
		add_child($3.ast_node, $4.ast_node); 
		libera($1.ast_node); 
	}
	| id '(' ')' { 
		// TODO: $$.ast_node = create_node("call "); 
		concat_label(&($$.ast_node->label), $1.table_entry.value.s); 
		libera($1.ast_node); 
	}
	| vector_index '(' expression arguments_list ')' { 
		$$.ast_node = create_node("call "); 
		// TODO: concat_label(&($$.ast_node->label), $1.ast_node->children[0]->label); 
		add_child($$.ast_node, $3.ast_node); 
		add_child($3.ast_node, $4.ast_node); 
		libera($1.ast_node); 
	}
	| vector_index '(' ')' { 
		$$.ast_node = create_node("call "); 
		// TODO: concat_label(&($$.ast_node->label), $1.ast_node->children[0]->label); 
		libera($1.ast_node); 
	}
	;
arguments_list: ',' expression arguments_list { 
		$$ = $2; 
		add_child($$.ast_node, $3.ast_node); 
	}
	| %empty { $$.ast_node = NULL; }
	;

// Comandos de shift
shift_left: id TK_OC_SL TK_LIT_INT { 
		$$.ast_node = create_node_lex_value($2); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, create_node_lex_value($3)); 
	}
	| vector_index TK_OC_SL TK_LIT_INT { 
		$$.ast_node = create_node_lex_value($2); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, create_node_lex_value($3)); 
	}
	;	
shift_right: id TK_OC_SR TK_LIT_INT { 
		$$.ast_node = create_node_lex_value($2); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, create_node_lex_value($3)); 
	}
	| vector_index TK_OC_SR TK_LIT_INT { 
		$$.ast_node = create_node_lex_value($2); 
		add_child($$.ast_node, $1.ast_node); 
		add_child($$.ast_node, create_node_lex_value($3)); 
	}
	;

// Comandos return, break e continue
return: TK_PR_RETURN expression { 
		$$.ast_node = create_node("return"); 
		add_child($$.ast_node, $2.ast_node); 
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

void yyerror(char const *s){
	printf("%s in line %d, column %d\n", s, line_number, column);
	exit(1);
}

