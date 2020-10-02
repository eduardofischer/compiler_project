%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "main.c"

extern int line_number, column;

int yylex(void);
void yyerror (char const *s);

AST_NODE * root = NULL;
%}

// Habilita o output verboso
%define parse.error verbose

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_PR_CONST
%token TK_PR_STATIC
%token TK_PR_FOREACH
%token TK_PR_FOR
%token TK_PR_SWITCH
%token TK_PR_CASE
%token TK_PR_BREAK
%token TK_PR_CONTINUE
%token TK_PR_CLASS
%token TK_PR_PRIVATE
%token TK_PR_PUBLIC
%token TK_PR_PROTECTED
%token TK_PR_END
%token TK_PR_DEFAULT
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_OC_SL
%token TK_OC_SR
%token TK_OC_FORWARD_PIPE
%token TK_OC_BASH_PIPE
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO

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
root: program			//{ root = $1; Print_Ast(root); } gera warning
	;
	
program: global_var_decl program 
	| function_def program
	| %empty
	;

type: TK_PR_INT		//{ $$ = Ast_Create(AST_SYMBOL_TK_PR_INT, 0, 0, 0, 0); }
	| TK_PR_FLOAT		//{ $$ = Ast_Create(AST_SYMBOL_TK_PR_FLOAT, 0, 0, 0, 0); }
	| TK_PR_BOOL		//{ $$ = Ast_Create(AST_SYMBOL_TK_PR_BOOL, 0, 0, 0, 0); }
	| TK_PR_CHAR		//{ $$ = Ast_Create(AST_SYMBOL_TK_PR_CHAR, 0, 0, 0, 0); }
	| TK_PR_STRING		//{ $$ = Ast_Create(AST_SYMBOL_TK_PR_STRING, 0, 0, 0, 0); }
	;

literal: TK_LIT_INT
	| TK_LIT_FLOAT
	| TK_LIT_FALSE
	| TK_LIT_TRUE
	| TK_LIT_CHAR
	| TK_LIT_STRING
	;

// Declaração de variáveis globais
global_var_decl: TK_PR_STATIC type TK_IDENTIFICADOR id_list ';'
	| type TK_IDENTIFICADOR id_list ';'
	| type TK_IDENTIFICADOR '[' TK_LIT_INT ']' id_list ';'
	;
id_list: ',' TK_IDENTIFICADOR id_list
	| ','  TK_IDENTIFICADOR '[' TK_LIT_INT ']' id_list
	| %empty
	;

// Definição de funções
function_def: type TK_IDENTIFICADOR '(' parameter parameters_list ')' cmd_block
	| type TK_IDENTIFICADOR '(' ')' cmd_block
	| TK_PR_STATIC type TK_IDENTIFICADOR '(' parameter parameters_list ')' cmd_block
	| TK_PR_STATIC type TK_IDENTIFICADOR '(' ')' cmd_block
	;
parameters_list: ',' parameter parameters_list
	| %empty
	;
parameter: type TK_IDENTIFICADOR
	| TK_PR_CONST type TK_IDENTIFICADOR
	;
cmd_block: '{' command_list '}'
	;

// Definição dos comandos dos blocos
commands: local_var_decl ';'
	| var_attribution ';'
	| function_call ';'
	| input ';'
	| output ';'
	| shift_left ';'
	| shift_right ';'
	| return ';'
	| break';'
	| continue ';'
	| conditional_if_else ';'
	| iterative_for_while ';'
	| cmd_block ';'
	;
command_list: commands command_list
	| %empty
	;
	
// Declaração de variaveis locais
local_var_decl: local_var_prefix type TK_IDENTIFICADOR local_list
	| local_var_prefix type TK_IDENTIFICADOR TK_OC_LE TK_IDENTIFICADOR local_list
	| local_var_prefix type TK_IDENTIFICADOR TK_OC_LE literal local_list
	;
local_var_prefix: TK_PR_STATIC
	| TK_PR_CONST
	| TK_PR_STATIC TK_PR_CONST
	| %empty
	;
local_list: ',' TK_IDENTIFICADOR local_list
	| ',' TK_IDENTIFICADOR TK_OC_LE TK_IDENTIFICADOR local_list
	| ',' TK_IDENTIFICADOR TK_OC_LE literal local_list
	| %empty
	;

// Atribuição de variavel
var_attribution: TK_IDENTIFICADOR '=' expression 
	| TK_IDENTIFICADOR '[' expression ']' '=' expression 
	;
	
// Expressões da linguagem
expression: TK_IDENTIFICADOR
	| TK_IDENTIFICADOR '[' expression ']'
	| function_call
	| literal
	| '(' expression ')'
	| unary_op expression %prec UNARY
	| expression binary_op expression %prec BINARY
	| expression '?' expression ':' expression %prec TERNARY
	;
unary_op:  '+' | '-' | '!' | '&' | '*' | '?' | '#';
binary_op: '+' | '-' | '*' | '/'	| '%'	| '^'							// Aritméticos
	| TK_OC_LE | TK_OC_GE | TK_OC_EQ | TK_OC_NE | '<' | '>' // Relacionais
	| TK_OC_OR | TK_OC_AND																	// Lógicos
	| '|' | '&'																							// Bitwise
	;

// Comandos de entrada e saida
input: TK_PR_INPUT TK_IDENTIFICADOR 
	;
output: TK_PR_OUTPUT TK_IDENTIFICADOR 
	| TK_PR_OUTPUT literal
	;
	
// Chamada de Função
function_call: TK_IDENTIFICADOR '(' expression arguments_list ')' 
	| TK_IDENTIFICADOR '(' ')'
	| TK_IDENTIFICADOR '[' expression ']' '(' expression arguments_list ')' 
	| TK_IDENTIFICADOR '[' expression ']' '(' ')' 
	;
arguments_list: ',' expression arguments_list
	| %empty
	;

// Comandos de shift
shift_left: TK_IDENTIFICADOR TK_OC_SL TK_LIT_INT
	| TK_IDENTIFICADOR '[' expression ']' TK_OC_SL TK_LIT_INT
	;	
shift_right: TK_IDENTIFICADOR TK_OC_SR TK_LIT_INT
	| TK_IDENTIFICADOR '[' expression ']' TK_OC_SR TK_LIT_INT
	;

// Comandos return, break e continue
return: TK_PR_RETURN expression;
break: TK_PR_BREAK;
continue: TK_PR_CONTINUE;

// Comandos de controle de fluxo
conditional_if_else: TK_PR_IF '(' expression ')' cmd_block
	| TK_PR_IF '(' expression ')' cmd_block TK_PR_ELSE cmd_block
	;
iterative_for_while: TK_PR_FOR '(' var_attribution ':' expression ':' var_attribution ')' cmd_block
	| TK_PR_WHILE '(' expression ')' TK_PR_DO cmd_block

%%

void yyerror(char const *s){
	printf("%s in line %d, column %d\n", s, line_number, column);
	exit(1);
}

