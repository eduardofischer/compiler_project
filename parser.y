%{
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "aux/ast.h"

extern int line_number, column;

int yylex(void);
void yyerror (char const *s);

extern void *arvore;
%}

// Habilita o output verboso
%define parse.error verbose

%union {
	AST_NODE *node;
	LEX_VALUE *valor_lexico;
}

%token <valor_lexico> TK_PR_INT
%token <valor_lexico> TK_PR_FLOAT
%token <valor_lexico> TK_PR_BOOL
%token <valor_lexico> TK_PR_CHAR
%token <valor_lexico> TK_PR_STRING
%token <valor_lexico> TK_PR_IF
%token <valor_lexico> TK_PR_ELSE
%token <valor_lexico> TK_PR_WHILE
%token <valor_lexico> TK_PR_DO
%token <valor_lexico> TK_PR_INPUT
%token <valor_lexico> TK_PR_OUTPUT
%token <valor_lexico> TK_PR_RETURN
%token <valor_lexico> TK_PR_CONST
%token <valor_lexico> TK_PR_STATIC
%token <valor_lexico> TK_PR_FOR
%token <valor_lexico> TK_PR_BREAK
%token <valor_lexico> TK_PR_CONTINUE
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
%type <node> id
%type <node> function_def
%type <node> cmd_block
%type <node> command_list
%type <node> command
%type <node> local_var_decl
%type <node> local_var_init
%type <node> var_attribution
%type <node> vector_index
%type <node> expression
%type <node> unary_op
%type <node> binary_op
%type <node> input
%type <node> output
%type <node> function_call
%type <node> shift_left
%type <node> shift_right
%type <node> return
%type <node> break 
%type <node> continue 
%type <node> conditional_if_else 
%type <node> iterative_for_while 

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
root: program	 { arvore = (void*) $1; print_ast((AST_NODE*)arvore); } 
	;
	
program: global_var_decl program { $$ = NULL; }
	| function_def program	{ $$ = $1; add_child($$, $2); }
	| %empty { $$ = NULL; }
	;

type: TK_PR_INT
	| TK_PR_FLOAT
	| TK_PR_BOOL
	| TK_PR_CHAR
	| TK_PR_STRING
	;

literal: TK_LIT_INT {$$ = create_node_lex_value($1);}
	| TK_LIT_FLOAT {$$ = create_node_lex_value($1);}
	| TK_LIT_FALSE {$$ = create_node_lex_value($1);}
	| TK_LIT_TRUE	{$$ = create_node_lex_value($1);}
	| TK_LIT_CHAR	{$$ = create_node_lex_value($1);}
	| TK_LIT_STRING	{$$ = create_node_lex_value($1);}
	;

// Declaração de variáveis globais
global_var_decl: TK_PR_STATIC type id id_list ';'
	| type id id_list ';'
	| type vector_index id_list ';'
	;
id_list: ',' id id_list
	| ',' vector_index id_list
	| %empty
	;
vector_index: id '[' expression ']' { $$ = create_node("[]"); add_child($$, $1); add_child($$, $3); }
id: TK_IDENTIFICADOR { $$ = create_node($1->value.s); }

	;
function_def: type id '(' parameter parameters_list ')' cmd_block { $$ =$2; add_child($$, $7); }
	| type id '(' ')' cmd_block { $$ = $2; add_child($$, $5); }
	| TK_PR_STATIC type id '(' parameter parameters_list ')' cmd_block { $$ = $3; add_child($$, $8); }
	| TK_PR_STATIC type id '(' ')' cmd_block { $$ = $3; add_child($$, $6); }
	;
parameters_list: ',' parameter parameters_list
	| %empty
	;
parameter: type id
	| TK_PR_CONST type id
	;

// Definição dos blocos de comandos
cmd_block: '{' command_list '}' { $$ = $2; }
	;
command_list: command command_list { if ($1 != NULL) {$$ = $1; add_child($$, $2);} else {$$ = $2;} }
	| %empty { $$ = NULL; }
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
local_var_decl: local_var_prefix type id local_list { $$ = NULL; }
	;
local_var_init: local_var_prefix type id TK_OC_LE id local_list { $$ = create_node("<="); add_child($$, $3); add_child($$, $5); }
	| local_var_prefix type id TK_OC_LE literal local_list { $$ = create_node("<="); add_child($$, $3); add_child($$, $5); }
	;
local_var_prefix: TK_PR_STATIC
	| TK_PR_CONST
	| TK_PR_STATIC TK_PR_CONST
	| %empty
	;
local_list: ',' id local_list
	| ',' id TK_OC_LE id local_list
	| ',' id TK_OC_LE literal local_list
	| %empty
	;

// Atribuição de variavel
var_attribution: id '=' expression { $$ = create_node("="); add_child($$, $1); add_child($$, $3); }
	| vector_index '=' expression { $$ = create_node("="); add_child($$, $1); add_child($$, $3); }
	;
	
// Expressões da linguagem
expression: id { $$ = $1; }
	| vector_index { $$ = $1; }
	| function_call  { $$ = $1; }
	| literal { $$ = $1; }
	| '(' expression ')'  { $$ = $2; }
	| unary_op expression %prec UNARY { $$ = $1; add_child($$, $2); }
	| expression binary_op expression %prec BINARY { $$ = $2; add_child($$, $1); add_child($$, $3); }
	| expression '?' expression ':' expression %prec TERNARY { $$ = create_node("?:"); add_child($$, $1); add_child($$, $3); add_child($$, $5);}
	;
unary_op: '+' { $$ = create_node("+"); }
	| '-' { $$ = create_node("-"); }
	| '!' { $$ = create_node("!"); }
	| '&' { $$ = create_node("&"); }
	| '*' { $$ = create_node("*"); }
	| '?' { $$ = create_node("?"); }
	| '#' { $$ = create_node("#"); }
	;
binary_op: '+' { $$ = create_node("+"); }
	| '-' { $$ = create_node("-"); }
	| '*' { $$ = create_node("*"); }
	| '/'	{ $$ = create_node("/"); }
	| '%'	{ $$ = create_node("%"); }
	| '^'	{ $$ = create_node("^"); }
	| TK_OC_LE { $$ = create_node("<="); }
	| TK_OC_GE { $$ = create_node(">="); }
	| TK_OC_EQ { $$ = create_node("=="); }
	| TK_OC_NE { $$ = create_node("!="); }
	| '<' { $$ = create_node("<"); }
	| '>' { $$ = create_node(">"); }
	| TK_OC_OR { $$ = create_node("||"); }
	| TK_OC_AND { $$ = create_node("&&"); }
	| '|' { $$ = create_node("|"); }
	| '&' { $$ = create_node("&"); }													
	;

// Comandos de entrada e saida
input: TK_PR_INPUT id { $$ = create_node("input"); add_child($$, $2); }
	;
output: TK_PR_OUTPUT id { $$ = create_node("output"); add_child($$, $2); }
	| TK_PR_OUTPUT literal { $$ = create_node("output"); add_child($$, $2); }
	;
	
// Chamada de Função
function_call: TK_IDENTIFICADOR '(' expression arguments_list ')' { $$ = create_node(strcat("call ", $1->value.s)); }
	| TK_IDENTIFICADOR '(' ')' { $$ = create_node(strcat("call ", $1->value.s)); }
	| vector_index '(' expression arguments_list ')' { $$ = create_node("call TODO"); }
	| vector_index '(' ')' { $$ = create_node("call TODO"); }
	;
arguments_list: ',' expression arguments_list
	| %empty
	;

// Comandos de shift
shift_left: id TK_OC_SL TK_LIT_INT { $$ = create_node("<<"); add_child($$, $1); add_child($$, create_node_lex_value($3)); }
	| vector_index TK_OC_SL TK_LIT_INT { $$ = create_node("<<"); add_child($$, $1); add_child($$, create_node_lex_value($3)); }
	;	
shift_right: id TK_OC_SR TK_LIT_INT { $$ = create_node(">>"); add_child($$, $1); add_child($$, create_node_lex_value($3)); }
	| vector_index TK_OC_SR TK_LIT_INT { $$ = create_node(">>"); add_child($$, $1); add_child($$, create_node_lex_value($3)); }
	;

// Comandos return, break e continue
return: TK_PR_RETURN expression { $$ = create_node("return"); add_child($$, $2); }
break: TK_PR_BREAK { $$ = create_node("break"); }
continue: TK_PR_CONTINUE { $$ = create_node("continue"); }

// Comandos de controle de fluxo
conditional_if_else: TK_PR_IF '(' expression ')' cmd_block { $$ = create_node("if"); add_child($$, $3); add_child($$, $5); }
	| TK_PR_IF '(' expression ')' cmd_block TK_PR_ELSE cmd_block { $$ = create_node("if"); add_child($$, $3); add_child($$, $5); add_child($$, $7); }
	;
iterative_for_while: TK_PR_FOR '(' var_attribution ':' expression ':' var_attribution ')' cmd_block { $$ = create_node("for"); add_child($$, $3); add_child($$, $5); add_child($$, $7); add_child($$, $9); }
	| TK_PR_WHILE '(' expression ')' TK_PR_DO cmd_block { $$ = create_node("while"); add_child($$, $3); add_child($$, $6); }

%%

void yyerror(char const *s){
	printf("%s in line %d, column %d\n", s, line_number, column);
	exit(1);
}

