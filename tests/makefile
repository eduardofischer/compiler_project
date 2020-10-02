CC = gcc
AUX = ../aux

all: test_ast

test_ast: test_ast.c $(AUX)/ast.c
	$(CC) -o test_ast test_ast.c $(AUX)/ast.c

clean:
	rm -rf test_ast