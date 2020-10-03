%{
	#include "types.h"
	#include "aux/ast.h"
	#include "parser.tab.h"

	int line_number = 1;
	int column = 1, next_column = 1;
	#define YY_USER_ACTION column = next_column; next_column += yyleng;
  
	void init_valor_lexico(int type);
%}

	/* Definicao de algumas expressoes */
DIGIT [0-9]
ALPHA [A-Za-z_]
SIGN [+-]
DELIMITER [ \n[:punct:]]

/* Por conta da segunda etapa possuir '+-', os inteiros aqui serão naturais */
INT {DIGIT}+	

CHAR \'[^']\'
STRING \"[^"\n]*\"
BOOL_LITERAL_TRUE "TRUE"|"true"
BOOL_LITERAL_FALSE "FALSE"|"false"
SPECIAL [\,\;\:\(\)\[\]\{\}\+\-\|\*\/\<\>\=\!\&\%\#\^\.\$\?]

%x MULTILINE_COMMENT

%%
 /* Guarda o numero de linhas */
\n {line_number++; next_column=1;}

 /* Caracteres que devem ser ignorados */
" " {}
"	" {}
\r {}

 /* Ignora comentarios simples */
\/\/.* {}

 /* Ignora comentarios multi-linha */
\/\* {BEGIN(MULTILINE_COMMENT);}							/* Entra no estado MULTILINE_COMMENT */
<MULTILINE_COMMENT>[^*\n]* {}               	/* Ignora qualquer CHAR que nao for * ou \n */
<MULTILINE_COMMENT>\*[^*/\n]* {}            	/* Ignora "*" nao seguido de "/" */
<MULTILINE_COMMENT>\n {line_number++;}       	/* Conta quebra de linhas em comentarios */
<MULTILINE_COMMENT>\*\/ {BEGIN(INITIAL);}    	/* Fim do comentario, retorna ao estado INITIAL */
 
  /* Identifica os literais da linguagem */
{INT}/{DELIMITER} {
		init_valor_lexico(TOKEN_TYPE_LIT);
		yylval.valor_lexico->literal_type = LIT_TYPE_INT;
		yylval.valor_lexico->value.i = atoi(yytext);
		return TK_LIT_INT;
	}

{INT}\.{INT}([Ee]{SIGN}?{INT})?/{DELIMITER} {
		init_valor_lexico(TOKEN_TYPE_LIT);
		yylval.valor_lexico->literal_type = LIT_TYPE_FLOAT;
		yylval.valor_lexico->value.f = atof(yytext);
		return TK_LIT_FLOAT;
	}

{CHAR}/{DELIMITER} {
		init_valor_lexico(TOKEN_TYPE_LIT);
		yylval.valor_lexico->literal_type = LIT_TYPE_CHAR;
		yylval.valor_lexico->value.c = yytext[1];
		return TK_LIT_CHAR;
	}

{STRING}/{DELIMITER} {
		init_valor_lexico(TOKEN_TYPE_LIT);
		yylval.valor_lexico->literal_type = LIT_TYPE_STRING;
		yylval.valor_lexico->value.s = strndup(yytext+1, strlen(yytext)-2);
		return TK_LIT_STRING;
	}

{BOOL_LITERAL_TRUE}/{DELIMITER} {
		init_valor_lexico(TOKEN_TYPE_LIT);
		yylval.valor_lexico->literal_type = LIT_TYPE_BOOL;
		yylval.valor_lexico->value.b = 1;
		return TK_LIT_TRUE;
	}

{BOOL_LITERAL_FALSE}/{DELIMITER} {
		init_valor_lexico(TOKEN_TYPE_LIT);
		yylval.valor_lexico->literal_type = LIT_TYPE_BOOL;
		yylval.valor_lexico->value.b = 0;
		return TK_LIT_FALSE;
	}
 
 /* Identifica as palavras reservadas da linguagem */
"int" {return TK_PR_INT;}
"float" {return TK_PR_FLOAT;}
"bool" {return TK_PR_BOOL;}
"char" {return TK_PR_CHAR;}
"string" {return TK_PR_STRING;}
"if" {return TK_PR_IF;}
"else" {return TK_PR_ELSE;}
"while" {return TK_PR_WHILE;}
"do" {return TK_PR_DO;}
"input" {return TK_PR_INPUT;}
"output" {return TK_PR_OUTPUT;}
"return" {return TK_PR_RETURN;}
"const" {return TK_PR_CONST;}
"static" {return TK_PR_STATIC;}
"for" {return TK_PR_FOR;}
"break" {return TK_PR_BREAK;}
"continue" {return TK_PR_CONTINUE;}
 
 /* Identifica os operadores compostos da linguagem */
"<=" {
		init_valor_lexico(TOKEN_TYPE_COMPOUND_OP);
		yylval.valor_lexico->value.s = strdup(yytext);
		return TK_OC_LE;
	}
">=" {
		init_valor_lexico(TOKEN_TYPE_COMPOUND_OP);
		yylval.valor_lexico->value.s = strdup(yytext);
		return TK_OC_GE;
	}
"==" {
		init_valor_lexico(TOKEN_TYPE_COMPOUND_OP);
		yylval.valor_lexico->value.s = strdup(yytext);
		return TK_OC_EQ;
	}
"!=" {
		init_valor_lexico(TOKEN_TYPE_COMPOUND_OP);
		yylval.valor_lexico->value.s = strdup(yytext);
		return TK_OC_NE;
	}
"&&" {
		init_valor_lexico(TOKEN_TYPE_COMPOUND_OP);
		yylval.valor_lexico->value.s = strdup(yytext);
		return TK_OC_AND;
	}
"||" {
		init_valor_lexico(TOKEN_TYPE_COMPOUND_OP);
		yylval.valor_lexico->value.s = strdup(yytext);
		return TK_OC_OR;
	}
">>" {
		init_valor_lexico(TOKEN_TYPE_COMPOUND_OP);
		yylval.valor_lexico->value.s = strdup(yytext);
		return TK_OC_SR;
	}
"<<" {
		init_valor_lexico(TOKEN_TYPE_COMPOUND_OP);
		yylval.valor_lexico->value.s = strdup(yytext);
		return TK_OC_SL;
	}
 
 /* Identifica os caracteres especiais da linguagem */
{SPECIAL} {
		init_valor_lexico(TOKEN_TYPE_SPECIAL);
		yylval.valor_lexico->value.c = yytext[0];
		return yytext[0];
	}

	/* Identifica os identificadores da linguagem */
{ALPHA}({ALPHA}|{DIGIT})* {
		init_valor_lexico(TOKEN_TYPE_ID);
		yylval.valor_lexico->value.s = strdup(yytext);
		return TK_IDENTIFICADOR;
	}

 /* Para qualquer outro padrão, retorna o token de erro */
. {return TOKEN_ERRO;}

%%

void init_valor_lexico(int type) {
	yylval.valor_lexico = malloc(sizeof(LEX_VALUE));
	yylval.valor_lexico->line_number = line_number;
	yylval.valor_lexico->token_type = type;
}