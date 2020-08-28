all: flex
	gcc main.c lex.yy.c -o etapa1 -lfl
	
flex: scanner.l
	flex scanner.l

clean:
	rm -rf etapa1 lex.yy.c
