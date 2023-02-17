%{
#include<string>
#include<iostream>

using namespace std;

#include "myclass.h"
#include "y.tab.h"
extern bool O_tokens;
%}

%option noyywrap

%%
bool {if(O_tokens){fprintf(yyout,"\tToken Name: BOOL\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 258;}
int {if(O_tokens){fprintf(yyout,"\tToken Name: INTEGER\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 259;}
float {if(O_tokens){fprintf(yyout,"\tToken Name: FLOAT\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 260;}
void {if(O_tokens){fprintf(yyout,"\tToken Name: VOID\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 261;}
string {if(O_tokens){fprintf(yyout,"\tToken Name: STRING\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 262;}
read {if(O_tokens){fprintf(yyout,"\tToken Name: READ\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 263;}
print {if(O_tokens){fprintf(yyout,"\tToken Name: WRITE\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 264;}
("\"")([^\"])*("\"") {if(O_tokens){fprintf(yyout,"\tToken Name: STR_CONST\tLexeme: %s\tLineno: %d\n",yytext,yylineno);} yylval.name= new string(yytext); return 265;}
[\n]
[\t ]+
[a-zA-Z_][a-zA-Z0-9_]*	{if(O_tokens){fprintf(yyout,"\tToken Name: NAME\tLexeme: %s\tLineno: %d\n",yytext,yylineno);} yylval.name= new string(yytext); return 266;}
[0-9]+ {if(O_tokens){fprintf(yyout,"\tToken Name: NUM\tLexeme: %s\tLineno: %d\n",yytext,yylineno);} yylval.num = atoi(yytext); return 267;}
[0-9]+\.[0-9]* {if(O_tokens){fprintf(yyout,"\tToken Name: FNUM\tLexeme: %s\tLineno: %d\n",yytext,yylineno);} yylval.fnum = atof(yytext); return 268;}
[0-9]*\.[0-9]+ {if(O_tokens){fprintf(yyout,"\tToken Name: FNUM\tLexeme: %s\tLineno: %d\n",yytext,yylineno);} yylval.fnum = atof(yytext); return 268;}
[+] {if(O_tokens){fprintf(yyout,"\tToken Name: PLUS\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 269;}
[-] {if(O_tokens){fprintf(yyout,"\tToken Name: MINUS\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return  270;}
[*] {if(O_tokens){fprintf(yyout,"\tToken Name: MULT\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 271  ;}
[/] {if(O_tokens){fprintf(yyout,"\tToken Name: DIV\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 272  ;}
[,] {if(O_tokens){fprintf(yyout,"\tToken Name: PUNCTUATION\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 273  ;}
[;] {if(O_tokens){fprintf(yyout,"\tToken Name: PUNCTUATION\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 274  ;}
[(] {if(O_tokens){fprintf(yyout,"\tToken Name: DELIMITERS\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 275  ;}
[)] {if(O_tokens){fprintf(yyout,"\tToken Name: DELIMITERS\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 276  ;}
[{] {if(O_tokens){fprintf(yyout,"\tToken Name: DELIMITERS\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 277  ;}
[}] {if(O_tokens){fprintf(yyout,"\tToken Name: DELIMITERS\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 278  ;}

[=] {if(O_tokens){fprintf(yyout,"\tToken Name: ASSIGN_OP\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 279  ;}

">=" {if(O_tokens){fprintf(yyout,"\tToken Name: GREATER_THAN_EQUAL\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 280  ;}

[>] {if(O_tokens){fprintf(yyout,"\tToken Name: GREATER_THAN\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 281  ;}

[<] {if(O_tokens){fprintf(yyout,"\tToken Name: LESS_THAN\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 282  ;}

"<=" {if(O_tokens){fprintf(yyout,"\tToken Name: LESS_THAN_EQUAL\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 283  ;}

"||" {if(O_tokens){fprintf(yyout,"\tToken Name: OR\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 284  ;}

"&&" {if(O_tokens){fprintf(yyout,"\tToken Name: AND\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 285  ;}

[!] {if(O_tokens){fprintf(yyout,"\tToken Name: NOT\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 286  ;}

"==" {if(O_tokens){fprintf(yyout,"\tToken Name: EQUAL\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 287  ;}

"!=" {if(O_tokens){fprintf(yyout,"\tToken Name: NOT_EQUAL\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 288  ;}

[:] {if(O_tokens){fprintf(yyout,"\tToken Name: OP_FOR_TERNARY_EXPR\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 289  ;}

[?] {if(O_tokens){fprintf(yyout,"\tToken Name: OP_FOR_TERNARY_EXPR\tLexeme: %s\tLineno: %d\n",yytext,yylineno);}return 290 ;}

\/\/(.*) ; 

. { fprintf(stderr,"illegal character\n"); exit(1);};

%%
