#PARSE= left-recursion-removed
PARSE= sclp
SCAN = sclp

sclp: lex.yy.o y.tab.o
	@gcc -o $(PARSE) lex.yy.o y.tab.o -ly -ll

y.tab.o y.tab.h: $(PARSE).y
	@yacc -dv $(PARSE).y
	@gcc -c y.tab.c

lex.yy.o : $(SCAN).l y.tab.h
	@lex $(SCAN).l
	@gcc -c lex.yy.c

clean:
	@rm -f lex.yy.* y.tab.* y.output sclp *.toks *.log a.out $(PARSE)
