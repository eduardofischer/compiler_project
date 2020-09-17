%{
#include <stdio.h>
#include <stdlib.h>
#include "main.c"

int yylex(void);
void yyerror (char const *s);
int get_line_number();
%}

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

%start program

%%

program: global_var_decl program
	| function_def program
	| %empty
	;
	
type: TK_PR_INT
	| TK_PR_FLOAT
	| TK_PR_BOOL
	| TK_PR_CHAR
	| TK_PR_STRING
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
	;

id_list: ',' TK_IDENTIFICADOR id_list
	| ','  TK_IDENTIFICADOR '[' TK_LIT_INT ']' id_list
	| %empty
	;

// Definição de funções
function_def: type TK_IDENTIFICADOR '(' parameter parameters_list ')' cmd_block
	| TK_PR_STATIC type TK_IDENTIFICADOR '(' parameter parameters_list ')' cmd_block
	;

parameters_list: ',' parameter parameters_list
	| %empty;
	;

parameter: type TK_IDENTIFICADOR
	| TK_PR_CONST type TK_IDENTIFICADOR
	| %empty;
	;

cmd_block: '{' cmd_commands_list '}'
	;

// Definição dos comandos dos blocos
cmd_commands: local_var_decl ';'
	| var_attribution ';'
	| input ';'
	| output ';'
	| function_call ';'
	;
	
cmd_commands_list:  cmd_commands cmd_commands_list
	| %empty
	;
	
// Declaração de variaveis locais
local_var_decl: TK_PR_STATIC type TK_IDENTIFICADOR id_list_local
	| TK_PR_STATIC TK_PR_CONST type TK_IDENTIFICADOR id_list_local
	| type TK_IDENTIFICADOR id_list_local 
	| type TK_IDENTIFICADOR '=' expression 		// Não sabia se realmente tinha um ponto de exclamação invertido antes do igual na definição, então só deixei o '=' por enquanto
	;
	
id_list_local: ',' TK_IDENTIFICADOR id_list_local
	| %empty
	;

// Atribuição de variavel
var_attribution: TK_IDENTIFICADOR '=' expression 
	| TK_IDENTIFICADOR '[' TK_LIT_INT ']' '=' expression 
	;
	
expression: TK_IDENTIFICADOR
	| literal
	;

// Comandos de entrada e saida
input: TK_PR_INPUT TK_IDENTIFICADOR 
	;
	
output: TK_PR_OUTPUT expression 
	;
	
// Chamada de Função
function_call: TK_IDENTIFICADOR '(' expression arguments_list ')' 
	| TK_IDENTIFICADOR '[' TK_LIT_INT ']' '(' expression arguments_list ')' 
	;

arguments_list: ',' expression arguments_list
	| %empty
	;

%%

void yyerror(char const *s){
	printf("%s in line number %d\n", s, get_line_number());
	exit(1);
}

