CC = gcc

AUX = ./aux

all: bison flex ast
	$(CC) -c lex.yy.c parser.tab.c
	$(CC) -o etapa3 lex.yy.o parser.tab.o ast.o -lfl
	
flex: scanner.l
	flex scanner.l
	
bison: parser.y
	bison -d parser.y --report=state
	
ast: $(AUX)/*.c
	$(CC) -c $(AUX)/*.c

clean:
	rm -rf etapa* lex.yy.c parser.tab.c parser.output parser.tab.h lex.yy.o parser.tab.o ast.o
