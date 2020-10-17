CC = gcc
AUX = ../aux

all: test_ast test_hashtable

test_ast: test_ast.c $(AUX)/ast.c
	$(CC) -o test_ast test_ast.c $(AUX)/ast.c

test_hashtable: test_hashtable.c $(AUX)/hashtable.c
	$(CC) -o test_hashtable test_hashtable.c $(AUX)/hashtable.c -g

clear:
	rm -rf test_ast test_hashtable