

PARSE = sclp
SCAN = sclp
OUT = sclp
FLAGS = -std=c++11


sclp	: lex.yy.o y.tab.o sclp.o ast.o tac.o
	@g++ $(FLAGS) -o $(OUT) sclp.o tac.o ast.o lex.yy.o y.tab.o -ly

sclp.o 	: sclp.cpp sclp.h ast.h tac.h
	@g++ $(FLAGS) -c sclp.cpp

ast.o 	: ast.cpp ast.h tac.h
	@g++ $(FLAGS) -c ast.cpp

tac.o 	: tac.cpp tac.h
	@g++ $(FLAGS) -c tac.cpp

y.tab.o y.tab.h  : $(PARSE).y ast.h tac.h
	@yacc -dv $(PARSE).y
	@g++ $(FLAGS) -c y.tab.c


lex.yy.o  : $(SCAN).l y.tab.h sclp.h ast.h tac.h
	@lex --yylineno $(SCAN).l
	@g++ $(FLAGS) -c lex.yy.c


clean :
	@rm -f lex.yy.* y.tab.*  *.o *.output a.out
	@rm -f $(OUT)
	@rm -f *.toks *.ast *.log *.tac
