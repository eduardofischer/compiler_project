%{
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "aux/ast.h"
#include "main.c"

extern int line_number, column;

int yylex(void);
void yyerror (char const *s);

AST_NODE *ast_root = NULL;
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
//%type <node> program
%type <node> type
%type <node> literal

// Apenas teste para conflitos de tipos por enquanto
%type <node> parameter 
%type <node> parameters_list



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
root: program	 //{ ast_root = $1; print_ast(ast_root); } //gera warning
	;
	
program: global_var_decl program 
	| function_def program	 
	| %empty
	;

type: TK_PR_INT		{$$ = create_node(AST_SYMBOL_TK_PR_INT);}
	| TK_PR_FLOAT		{$$ = create_node(AST_SYMBOL_TK_PR_FLOAT);}
	| TK_PR_BOOL		{$$ = create_node(AST_SYMBOL_TK_PR_BOOL);}
	| TK_PR_CHAR		{$$ = create_node(AST_SYMBOL_TK_PR_CHAR);}
	| TK_PR_STRING		{$$ = create_node(AST_SYMBOL_TK_PR_STRING);}
	;

literal: TK_LIT_INT		{$$ = create_node(AST_SYMBOL_TK_LIT_INT);}
	| TK_LIT_FLOAT		{$$ = create_node(AST_SYMBOL_TK_LIT_FLOAT);}
	| TK_LIT_FALSE		{$$ = create_node(AST_SYMBOL_TK_LIT_FALSE);}
	| TK_LIT_TRUE		{$$ = create_node(AST_SYMBOL_TK_LIT_TRUE);}
	| TK_LIT_CHAR		{$$ = create_node(AST_SYMBOL_TK_LIT_CHAR);}
	| TK_LIT_STRING	{$$ = create_node(AST_SYMBOL_TK_LIT_STRING);}
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
parameters_list: ',' parameter parameters_list	{$$ = create_node(AST_SYMBOL_parameters_list); add_child($$, $2); add_child($$, $3);}		// Modelo que pensei para fazer os nodes com seus filhos
	| %empty					{$$ = NULL;}
	;
parameter: type TK_IDENTIFICADOR			{$$ = create_node(AST_SYMBOL_parameter); add_child($$, $1);}
	| TK_PR_CONST type TK_IDENTIFICADOR		{$$ = create_node(AST_SYMBOL_const_parameter); add_child($$, $2);}
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

