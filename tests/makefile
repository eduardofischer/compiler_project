CC = gcc
AUX = ../aux

all: test_ast test_hashtable test_stack

test_ast: test_ast.c $(AUX)/ast.c
	$(CC) -o test_ast test_ast.c $(AUX)/ast.c -g

test_hashtable: test_hashtable.c $(AUX)/hashtable.c
	$(CC) -o test_hashtable test_hashtable.c $(AUX)/hashtable.c -g

test_stack: test_stack.c $(AUX)/stack.c
	$(CC) -o test_stack test_stack.c $(AUX)/hashtable.c $(AUX)/stack.c -g

clear:
	rm -rf test_ast test_hashtable test_stack