CC = gcc
AUX = ./aux

all: bison flex aux_ast aux_hash aux_errors aux_stack
	$(CC) -c lex.yy.c parser.tab.c main.c -g
	$(CC) -o etapa4 *.o -lfl -g
	
flex: scanner.l
	flex scanner.l
	
bison: parser.y
	bison -d parser.y --report=state
	
aux_ast: aux/ast.c 
	$(CC) -c aux/ast.c -g
	
aux_hash: aux/hashtable.c
	$(CC) -c aux/hashtable.c -g

aux_errors: aux/errors.c
	$(CC) -c aux/errors.c -g
	 
aux_stack: aux/stack.c
	$(CC) -c aux/stack.c -g

clear:
	rm -rf etapa* lex.yy.c parser.tab.c parser.output parser.tab.h lex.yy.o parser.tab.o main.o ast.o hashtable.o stack.o errors.o
