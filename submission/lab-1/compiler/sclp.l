%{
#include <stdio.h>
#include "y.tab.h"
extern int is_print;
%}
%%
bool if(is_print==1){fprintf(yyout,"\tToken Name: BOOL\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 258;
int if(is_print==1){fprintf(yyout,"\tToken Name: INTEGER\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 259;
float if(is_print==1){fprintf(yyout,"\tToken Name: FLOAT\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 260;
void if(is_print==1){fprintf(yyout,"\tToken Name: VOID\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 261;
string if(is_print==1){fprintf(yyout,"\tToken Name: STRING\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 262;
read if(is_print==1){fprintf(yyout,"\tToken Name: READ\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 263;
print if(is_print==1){fprintf(yyout,"\tToken Name: WRITE\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 264;
("\"")(.)*("\"") if(is_print==1){fprintf(yyout,"\tToken Name: STR_CONST\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 265;
[\n] yylineno++;
[\t ]+
[a-zA-Z_][a-zA-Z0-9_]*	if(is_print==1){fprintf(yyout,"\tToken Name: NAME\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 266;
[0-9]+ if(is_print==1){fprintf(yyout,"\tToken Name: NUM\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 267;
[0-9]+\.[0-9]+ if(is_print==1){fprintf(yyout,"\tToken Name: FNUM\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 268;
[+] if(is_print==1){fprintf(yyout,"\tToken Name: PLUS\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 269;
[-] if(is_print==1){fprintf(yyout,"\tToken Name: MINUS\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 270;
[*] if(is_print==1){fprintf(yyout,"\tToken Name: MULT\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 271;
[/] if(is_print==1){fprintf(yyout,"\tToken Name: DIV\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 272;
[,] if(is_print==1){fprintf(yyout,"\tToken Name: PUNCTUATION\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 273;
[;] if(is_print==1){fprintf(yyout,"\tToken Name: PUNCTUATION\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 274;
[(] if(is_print==1){fprintf(yyout,"\tToken Name: DELIMITERS\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 275;
[)] if(is_print==1){fprintf(yyout,"\tToken Name: DELIMITERS\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 276;
[{] if(is_print==1){fprintf(yyout,"\tToken Name: DELIMITERS\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 277;
[}] if(is_print==1){fprintf(yyout,"\tToken Name: DELIMITERS\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 278;
= if(is_print==1){fprintf(yyout,"\tToken Name: ASSIGN_OP\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 279;
\/\/(.*) ; 
. ;
%%

