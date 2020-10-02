all: bison flex ast
	gcc -c lex.yy.c parser.tab.c
	gcc -o etapa3 lex.yy.o parser.tab.o ast.o -lfl
	
flex: scanner.l
	flex scanner.l
	
bison: parser.y
	bison -d parser.y --report=state
	
ast: ast.c
	gcc -c ast.c

clean:
	rm -rf etapa2 lex.yy.c parser.tab.c parser.output parser.tab.h lex.yy.o parser.tab.o ast.o
