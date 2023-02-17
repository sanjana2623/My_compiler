#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include "ast.h"    
using namespace std;

// external variable ###########################################
extern FILE *yyin, *yyout;
extern int yylex();
extern int yyparse();

// global variable #############################################
bool  O_tokens = false, O_ast = false, O_tac = false, O_rtl = false ;
bool O_sa_scan=false, O_sa_parse=false, O_sa_ast=false, O_sa_tac=false;

bool is_def = false;
int temp_index = 0;
int stemp_index = -1;
int label_index = 0;
int str_index=-1;

vector<int> temp_toReg;    //for temp0 to v0 conversion

Program_Node* program_root = new Program_Node(); //IMP for holding list of AST & TAC
Param_class* PC_def = new Param_class(); //for holding param list (def)
Param_class* PC_declr = new Param_class(); //for holding param list (declr)

Reg_alloc_class* Reg_alloc_obj = new Reg_alloc_class();

/////////////////////////////////////////////////////
int main(int argc, char const *argv[]){
	char input_file[256];
	char tok_file[256];
	char ast_file[256];
	char tac_file[256];
	char rtl_file[256];


	for(int i=1; i<argc; i++){
		if(argv[i][0] != '-'){
			strcpy(input_file, argv[i]);
		}
		else if(strcmp(argv[i], "-tokens") == 0)  { O_tokens = true; }
		else if(strcmp(argv[i], "-ast") == 0)     { O_ast = true; }
		else if(strcmp(argv[i], "-tac") == 0)     { O_tac = true; }
		else if(strcmp(argv[i], "-rtl") == 0)     { O_rtl = true; }
		else if(strcmp(argv[i], "-sa-scan") == 0) { O_sa_scan = true; }
		else if(strcmp(argv[i], "-sa-parse") == 0){ O_sa_parse = true; }
		else if(strcmp(argv[i], "-sa-ast") == 0)  { O_sa_ast = true; }
		else if(strcmp(argv[i], "-sa-tac") == 0)  { O_sa_tac = true; }
		else{}
	}

	strcpy(tok_file, input_file);
	strcpy(ast_file, input_file);
	strcpy(tac_file, input_file);
	strcpy(rtl_file, input_file);

	strcat(tok_file, ".toks");
	strcat(ast_file, ".ast");
	strcat(tac_file, ".tac");
	strcat(rtl_file, ".rtl");

	ofstream aout, tout, rout;
	if(O_ast){
		aout.open(string(ast_file), ios::trunc);
	}
	if(O_tac){
		tout.open(string(tac_file), ios::trunc);
	}
	if(O_rtl){
		rout.open(string(rtl_file), ios::trunc);
	}

	yyin = fopen(input_file, "r");

	if(O_tokens){
		yyout = fopen(tok_file, "w");
	}
	else{}

	if(O_sa_scan){
		while(yylex())
			;
		return 0;
	}

	yyparse();
	if(O_sa_parse){
		return 0;
	}

	if(O_ast){
		program_root->makeAstFile(aout);
	}

	if(O_sa_ast){
		return 0;
	}

	if(O_tac){
		program_root->makeTacFile(tout);
	}

	if(O_sa_tac){
		return 0;
	}

	if(O_rtl){
		program_root->makeRtlFile(rout);
	}
	aout.close();
	tout.close();
	rout.close();
	return 0;
}

/////////////////////////////////////////////////////
void yyerror(const char *mesg){
	extern int yylineno;
	fprintf(stderr,"%s at line %d\n",mesg,yylineno);
	exit(1);
}

int yywrap(){
	return 1;
}

void semantic_error(){
	fprintf(stderr,"semantic error\n");
	exit(1);
}
/////////////////////////////////////////////////////


// function definition #########################################
void printToken(const char* toksname, char* yytext,int yylineno){
	if(O_tokens){
		fprintf(yyout,"\tToken Name: %s\tLexeme: %s\tLineno: %d\n",
				toksname, yytext,yylineno);
	}
}

void printEnumDT(ostream& fout, DataType dtype){
	switch(dtype){
		case INTe : {
			fout<<"<int>";
			break;
		}
		case FLOATe : {
			fout<<"<float>";
			break;
		}
		case BOOLe : {
			fout<<"<bool>";
			break;
		}
		case STRINGe : {
			fout<<"<string>";
			break;
		}
	}
}

void printEnumIO(ostream& fout,NodeText label){
	switch(label){
		case READ_E : {
			fout<<"Read: ";
			break;
		}
		case PRINT_E : {
			fout<<"Write: ";
			break;
		}
	}
}

void printEnumArith(ostream& fout,NodeText label){
	switch(label){
		case Plus_E : {
			fout<<"Plus";
			break;
		}
		case Minus_E : {
			fout<<"Minus";
			break;
		}
		case Mult_E : {
			fout<<"Mult";
			break;
		}
		case Div_E : {
			fout<<"Div";
			break;
		}
		case Uminus_E : {
			fout<<"Uminus";
			break;
		}
	}
}

void printEnumRelat(ostream& fout,NodeText label){
	switch(label){
		case GT_E : {
			fout<<"GT<bool>";
			break;
		}
		case GTE_E : {
			fout<<"GE<bool>";
			break;
		}
		case LT_E : {
			fout<<"LT<bool>";
			break;
		}
		case LTE_E : {
			fout<<"LE<bool>";
			break;
		}
		case Or_E : {
			fout<<"OR<bool>";
			break;
		}
		case And_E : {
			fout<<"AND<bool>";
			break;
		}
		case EQ_E : {
			fout<<"EQ<bool>";
			break;
		}
		case NEQ_E : {
			fout<<"NE<bool>";
			break;
		}
		case Not_E : {
			fout<<"NOT<bool>";
			break;
		}
	}
}

void printEnumOp(ostream& fout,NodeText label){
	switch(label){
		// arith Operator
		case Plus_E : {
			fout<<"+";
			break;
		}
		case Minus_E : {
			fout<<"-";
			break;
		}
		case Mult_E : {
			fout<<"*";
			break;
		}
		case Div_E : {
			fout<<"/";
			break;
		}
		case Uminus_E : {
			fout<<"-";
			break;
		}

		// relational Operator
		case GT_E : {
			fout<<">";
			break;
		}
		case GTE_E : {
			fout<<">=";
			break;
		}
		case LT_E : {
			fout<<"<";
			break;
		}
		case LTE_E : {
			fout<<"<=";
			break;
		}
		case Or_E : {
			fout<<"||";
			break;
		}
		case And_E : {
			fout<<"&&";
			break;
		}
		case EQ_E : {
			fout<<"==";
			break;
		}
		case NEQ_E : {
			fout<<"!=";
			break;
		}
		case Not_E : {
			fout<<"!";
			break;
		}
	}
}
