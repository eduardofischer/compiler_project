%{
	#include <stdio.h>
	#include "tokens.h"
	
	int line_number = 1;

%}

 /* Definicao de algumas expressoes */
digit [0-9]     
alphabet [A-Za-z]
integer ("-"|"+")?{digit}+
char \'[^']\'
string \"[^"]*\"

%%
 /* Guarda o numero de linhas */
"\n" {line_number += 1;}

 /* Caracteres que devem ser ignorados */
" " {}
"	" {}
"\r" {}

 /* Ignora comentarios */
\/\/.* {}
\/\*.*\*\/ {}
 
  /* Identifica os literais da linguagem */
{integer} {return TK_LIT_INT;}     

{integer}("."{digit}+)?(("E"|"e"){integer})? {return TK_LIT_FLOAT;}

{char} {return TK_LIT_CHAR;}

{string} {return TK_LIT_STRING;}
 
 
 /* Identifica as palavras reservadas da linguagem */
"int" {return TK_PR_INT;}
"float" {return TK_PR_FLOAT;}
"bool" {return TK_PR_BOOL;}
"char" {return TK_PR_CHAR;}
"string" {return TK_PR_STRING;}
"if" {return TK_PR_IF;}
"then" {return TK_PR_THEN;}
"else" {return TK_PR_ELSE;}
"while" {return TK_PR_WHILE;}
"do" {return TK_PR_DO;}
"input" {return TK_PR_INPUT;}
"output" {return TK_PR_OUTPUT;}
"return" {return TK_PR_RETURN;}
"const" {return TK_PR_CONST;}
"static" {return TK_PR_STATIC;}
"foreach" {return TK_PR_FOREACH;}
"for" {return TK_PR_FOR;}
"switch" {return TK_PR_SWITCH;}
"case" {return TK_PR_CASE;}
"break" {return TK_PR_BREAK;}
"continue" {return TK_PR_CONTINUE;}
"class" {return TK_PR_CLASS;}
"public" {return TK_PR_PUBLIC;}
"private" {return TK_PR_PRIVATE;}
"protected" {return TK_PR_PROTECTED;}
"end" {return TK_PR_END;}
"default" {return TK_PR_DEFAULT;}
 
 /* Identifica os operadores compostos da linguagem */
"<=" {return TK_OC_LE;}
">=" {return TK_OC_GE;}
"==" {return TK_OC_EQ;}
"!=" {return TK_OC_NE;}
"&&" {return TK_OC_AND;}
"||" {return TK_OC_OR;}
">>" {return TK_OC_SL;}
"<<" {return TK_OC_SR;}
 
 /* Identifica os caracteres especiais da linguagem */
"," {return ',';}
";" {return ';';}
":" {return ':';}
"(" {return '(';}
")" {return ')';}
"[" {return '[';}
"]" {return ']';}
"{" {return '{';}
"}" {return '}';}
"+" {return '+';}
"-" {return '-';}
"|" {return '|';}
"*" {return '*';}
"/" {return '/';}
"<" {return '<';}
">" {return '>';}
"=" {return '=';}
"!" {return '!';}
"&" {return '&';}
"%" {return '%';}
"#" {return '#';}
"ˆ" {return 'ˆ';}    /* Gera warning - multi-character character constant */
"." {return '.';}
"$" {return '$';}


 /* Identifica os identificadores da linguagem */
 {alphabet}({alphabet}|{digit})* {return TK_IDENTIFICADOR;}

%%

 /* Funcao que retorna a contagem de linhas */
int get_line_number(){
	return line_number;
}
