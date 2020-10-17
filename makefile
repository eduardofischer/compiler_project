CC = gcc
AUX = ./aux

all: bison flex ast
	$(CC) -c lex.yy.c parser.tab.c main.c
	$(CC) -o etapa3 lex.yy.o parser.tab.o ast.o main.o -lfl
	
flex: scanner.l
	flex scanner.l
	
bison: parser.y
	bison -d parser.y
	
ast: $(AUX)/ast.c
	$(CC) -c $(AUX)/ast.c

clear:
	rm -rf etapa* lex.yy.c parser.tab.c parser.output parser.tab.h lex.yy.o parser.tab.o main.o ast.o
