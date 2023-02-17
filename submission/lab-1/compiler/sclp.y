%{
#include<stdio.h>
#include <string.h>

int yylex();
void yyerror(const char *mesg);
int return_val  = 0;
int is_print;
// only print when is_print = 1
%}

%start program
%token BOOL INTEGER FLOAT VOID STRING 
%token READ WRITE 
%token STR_CONST NAME NUM FNUM
%token PLUS MINUS MULT DIV 
%token COMMA COLON 
%token S_O S_C M_O M_C
%token ASSIGN_OP 
%left PLUS MINUS
%left DIV MULT

%%
program: glob_declr proc_def;
glob_declr: |dtype varlist COLON var_declr_list;
proc_def: VOID NAME S_O S_C M_O var_declr_list stat_list M_C;
var_declr_list: |dtype varlist COLON var_declr_list ; 
stat_list: |stat_list WRITE wrt COLON
           |stat_list READ rd COLON
	   |stat_list NAME ASSIGN_OP exp COLON;
varlist: NAME
	|varlist COMMA NAME;
rd: NAME | NUM | FNUM;
wrt: NAME | NUM | FNUM | STR_CONST;
exp:	NAME	
	| NUM	
	| FNUM  
        | STR_CONST 
	| exp PLUS exp	
	| exp MINUS exp 
	| exp MULT exp 
	| exp DIV exp 
	| S_O exp S_C 
	| MINUS exp;
dtype: INTEGER
	| FLOAT
	| STRING
	| BOOL;

%%


int main(int argc, char const *argv[]){
	extern FILE *yyin, *yyout;

	if(argc == 2){
		is_print = 0;
		yyin = fopen(argv[1], "r");
		yyparse();

	}
	else if(argc == 3){
		if (strcmp(argv[1],"-tokens") == 0){ 
			is_print = 1;

			yyin = fopen(argv[2], "r");

			char output_file[256];
			strcpy(output_file, argv[2]);
			strcat(output_file, ".toks");

			yyout = fopen(output_file, "w");
			yyparse(); 
		}
		else if (strcmp(argv[2],"-tokens") == 0){ 
			is_print = 1;

			yyin = fopen(argv[1], "r");

			char output_file[256];
			strcpy(output_file, argv[1]);
			strcat(output_file, ".toks");

			yyout = fopen(output_file, "w");
			yyparse(); 
		}
		else{
			is_print = 0;
			yyin = fopen(argv[2], "r");
			yyparse();
			return 1;
		}
	}
	else{
		return 1;
	}


	return return_val;
}

void yyerror(const char *mesg){
	return_val = 1;
	fprintf(stderr,"%s\n",mesg);
}

int yywrap(){
	return 1;
}
