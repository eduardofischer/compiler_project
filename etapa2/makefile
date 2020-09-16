all: bison flex
	gcc -c lex.yy.c parser.tab.c
	gcc -o etapa2 lex.yy.o parser.tab.o -lfl
	
flex: scanner.l
	flex scanner.l
	
bison: parser.y
	bison -d parser.y

clean:
	rm -rf etapa2 lex.yy.c parser.tab.c parser.output parser.tab.h lex.yy.o parser.tab.o
