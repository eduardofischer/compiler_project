CC = gcc
AUX = ./aux

all: bison flex aux_ast aux_hash aux_errors aux_stack aux_iloc aux_asm
	$(CC) -c lex.yy.c parser.tab.c main.c -g
	$(CC) -o etapa6 *.o -lfl -g
	
flex: scanner.l
	flex scanner.l
	
bison: parser.y
	bison -d parser.y --report=state
	
aux_ast: aux/ast.c 
	$(CC) -c aux/ast.c -g

aux_asm: aux/asm.c 
	$(CC) -c aux/asm.c -g
	
aux_hash: aux/hashtable.c
	$(CC) -c aux/hashtable.c -g

aux_errors: aux/errors.c
	$(CC) -c aux/errors.c -g
	 
aux_stack: aux/stack.c
	$(CC) -c aux/stack.c -g

aux_iloc: aux/iloc_code_gen.c
	$(CC) -c aux/iloc_code_gen.c -g

clear:
	rm -rf etapa* lex.yy.c parser.tab.c parser.output parser.tab.h lex.yy.o parser.tab.o main.o ast.o asm.o hashtable.o stack.o errors.o iloc_code_gen.o
