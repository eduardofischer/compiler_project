CC = gcc
AUX = ./aux

all: bison flex aux
	$(CC) -c lex.yy.c parser.tab.c main.c -g
	$(CC) -o etapa4 *.o -lfl -g
	
flex: scanner.l
	flex scanner.l
	
bison: parser.y
	bison -d parser.y
	
aux: $(AUX)/*.c
	$(CC) -c $(AUX)/*.c

clear:
	rm -rf etapa* lex.yy.c parser.tab.c parser.output parser.tab.h lex.yy.o parser.tab.o main.o ast.o
