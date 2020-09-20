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


// ## Associatividade e prioridade dos operadores

// Virgula
%left ','
// Atribuição
%right '='
// Operador Condicional
%left '?' ':'
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
	| type TK_IDENTIFICADOR '[' TK_LIT_INT ']' id_list ';'
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
	;
	
cmd_commands_list: cmd_commands cmd_commands_list
	| %empty
	;
	
// Declaração de variaveis locais
local_var_decl: local_var_prefix type TK_IDENTIFICADOR id_list_local
	| local_var_prefix type TK_IDENTIFICADOR '=' expression 		// Não sabia se realmente tinha um ponto de exclamação invertido antes do igual na definição, então só deixei o '=' por enquanto
	;

local_var_prefix: TK_PR_STATIC
	| TK_PR_CONST
	| TK_PR_STATIC TK_PR_CONST
	| %empty
	;
	
id_list_local: ',' TK_IDENTIFICADOR id_list_local
	| %empty
	;

// Atribuição de variavel
var_attribution: TK_IDENTIFICADOR '=' expression 
	| TK_IDENTIFICADOR '[' expression ']' '=' expression 
	;
	
// Expressões da linguagem
expression: TK_IDENTIFICADOR { printf("id\n"); }
	| TK_IDENTIFICADOR '[' expression ']' { printf("vetor\n"); }
	| function_call { printf("function call\n"); }
	| literal { printf("literal\n"); }
	| '(' expression ')' { printf("parentesis\n"); }
	| unary_op expression %prec UNARY { printf("unary\n"); }
	| expression binary_op expression %prec BINARY { printf("binary\n"); }
	| expression '?' expression ':' expression %prec TERNARY { printf("ternary\n"); }
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
	| TK_IDENTIFICADOR '[' expression ']' '(' expression arguments_list ')' 
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
return: TK_PR_RETURN expression
	;
break: TK_PR_BREAK
	;
continue: TK_PR_CONTINUE
	;

// Comandos de controle de fluxo
conditional_if_else: TK_PR_IF '(' expression ')' cmd_block
	| TK_PR_IF '(' expression ')' cmd_block TK_PR_ELSE cmd_block
	;

iterative_for_while: TK_PR_FOR '(' var_attribution ':' expression ':' var_attribution ')' cmd_block
	| TK_PR_WHILE '(' expression ')' cmd_block



%%

void yyerror(char const *s){
	printf("%s in line number %d\n", s, get_line_number());
	exit(1);
}

