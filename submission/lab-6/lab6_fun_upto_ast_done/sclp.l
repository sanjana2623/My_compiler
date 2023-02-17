%{
#include<string>
#include<iostream>
#include "sclp.h"
#include "y.tab.h"

using namespace std;

%}

%option noyywrap

%%
if {printToken("IF", yytext, yylineno); return 258;}

else {printToken("ELSE", yytext, yylineno); return 259;}

while {printToken("WHILE", yytext, yylineno); return 260;}

do {printToken("DO", yytext, yylineno); return 261;}

bool {printToken("BOOL", yytext, yylineno); return 262;}

int {printToken("INTEGER", yytext, yylineno); return 263;}

float {printToken("FLOAT", yytext, yylineno); return 264;}

void {printToken("VOID", yytext, yylineno); return 265;}

string {printToken("STRING", yytext, yylineno); return 266;}

read {printToken("READ", yytext, yylineno); return 267;}

print {printToken("WRITE", yytext, yylineno); return 268;}

return {printToken("RETURN", yytext, yylineno); return 269;}

("\"")([^\"])*("\"") {printToken("STR_CONST", yytext, yylineno); yylval.name= new string(yytext); return 270;}

[\n]
[\t ]+

[a-zA-Z_][a-zA-Z0-9_]*	{printToken("NAME", yytext, yylineno); yylval.name= new string(yytext); return 271;}

[0-9]+ {printToken("NUM", yytext, yylineno); yylval.num = atoi(yytext); return 272;}

[0-9]+\.[0-9]* {printToken("FNUM", yytext, yylineno); yylval.fnum = atof(yytext); return 273;}

[0-9]*\.[0-9]+ {printToken("FNUM", yytext, yylineno); yylval.fnum = atof(yytext); return 273;}

[+] {printToken("PLUS", yytext, yylineno); return 274;}

[-] {printToken("MINUS", yytext, yylineno); return 275;}

[*] {printToken("MULT", yytext, yylineno); return 276;}

[/] {printToken("DIV", yytext, yylineno); return 277;}

[,] {printToken("PUNCTUATION", yytext, yylineno); return 278;}

[;] {printToken("PUNCTUATION", yytext, yylineno); return 279;}

[(] {printToken("DELIMITERS", yytext, yylineno); return 280;}

[)] {printToken("DELIMITERS", yytext, yylineno); return 281;}

[{] {printToken("DELIMITERS", yytext, yylineno); return 282;}

[}] {printToken("DELIMITERS", yytext, yylineno); return 283;}

[=] {printToken("ASSIGN_OP", yytext, yylineno); return 284;}

">=" {printToken("GREATER_THAN_EQUAL", yytext, yylineno); return 285;}

[>] {printToken("GREATER_THAN", yytext, yylineno); return 286;}

[<] {printToken("LESS_THAN", yytext, yylineno); return 287;}

"<=" {printToken("LESS_THAN_EQUAL", yytext, yylineno); return 288;}

"||" {printToken("OR", yytext, yylineno); return 289;}

"&&" {printToken("AND", yytext, yylineno); return 290;}

[!] {printToken("NOT", yytext, yylineno); return 291;}

"==" {printToken("EQUAL", yytext, yylineno); return 292;}

"!=" {printToken("NOT_EQUAL", yytext, yylineno); return 293;}

[:] {printToken("OP_FOR_TERNARY_EXPR", yytext, yylineno); return 294;}

[?] {printToken("OP_FOR_TERNARY_EXPR", yytext, yylineno); return 295;}

\/\/(.*) ; 

. {fprintf(stderr,"illegal character\n"); exit(1);};

%%
