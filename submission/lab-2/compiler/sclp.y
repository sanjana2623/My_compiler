%{
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include "myclass.h"	
using namespace std;

extern FILE *yyin, *yyout;
bool  O_tokens = false, O_ast= false, O_sa_scan=false, O_sa_parse=false;

int yylex();
void yyerror(const char *mesg);
void semantic_error();
bool is_def=false;

vector<string*> var_vec;  // for varlist

vector<string*>  param_var1;  // for var name of parameter list
vector<string*>  param_var2; 
vector<DataType> param_dt1;  // for dtype of parameter list
vector<DataType> param_dt2; 


Program_Node* program_root = new Program_Node(); //IMP for holding list of AST
Symbol_Table symbol_TableO;       // symbol table object consist of all sym table
%}

%union{
	int num;
	float fnum;
	string * name;
	enum DataType dtype;
	class Exp_Ast* exp_Ast;
}

%start program

%token BOOL INTEGER FLOAT VOID STRING 
%token READ WRITE 
%token <name> STR_CONST <name> NAME <num> NUM <fnum> FNUM
%token PLUS MINUS MULT DIV 
%token COMMA SCOLON 
%token S_O S_C M_O M_C
%token ASSIGN_OP 
%token GREATER_THAN_EQUAL GREATER_THAN LESS_THAN LESS_THAN_EQUAL
%token OR AND NOT EQUAL NOT_EQUAL
%token COLON QUES

%right QUES COLON
%left OR
%left AND
%left NOT
%left NOT_EQUAL EQUAL LESS_THAN LESS_THAN_EQUAL GREATER_THAN GREATER_THAN_EQUAL
%left PLUS MINUS
%left DIV MULT
%right UMINUS

%type <dtype> dtype
%type <exp_Ast> exp

%%
program: glob_declr fun_def proc_def
	|fun_def glob_declr proc_def
	|glob_declr fun_def glob_declr proc_def
	|glob_declr proc_def
	|fun_def proc_def
	|proc_def;

glob_declr: var_declrG | glob_declr var_declrG ;
var_declrG: dtype varlistG SCOLON 
	{
	if(!O_sa_parse){ 
		for(auto v:var_vec){
			symbol_TableO.insert_G(v, $1);
		}
		var_vec.clear();
	}
	};

varlistG: varlistG COMMA NAME
	{
	if(!O_sa_parse){ 
		var_vec.push_back($3);
	}
	} 
	|NAME 
	{
	if(!O_sa_parse){ 
		var_vec.push_back($1);
	}
	};

fun_def: VOID NAME S_O S_C SCOLON
	{
	if(!O_sa_parse){ 
		is_def = true;
		if(*$2 != "main"){
			semantic_error();
		}
	}
	}
	|VOID NAME S_O param_list S_C SCOLON 
	{
	if(!O_sa_parse){ 
		is_def = true;
		if(*$2 != "main"){
			semantic_error();
		}
	}
	}
	|dtype NAME S_O param_list S_C SCOLON
	{
	if(!O_sa_parse){ 
		semantic_error();
	}
	}
	|dtype NAME S_O S_C SCOLON
	{
	if(!O_sa_parse){ 
		semantic_error();
	}
	}
	;


proc_def:  dtype NAME S_O S_C M_O var_declr_list stat_list M_C
	{
	if(!O_sa_parse){ 
		semantic_error();
	}
	}
	|dtype NAME S_O param_list S_C M_O var_declr_list stat_list M_C 
	{
	if(!O_sa_parse){ 
		semantic_error();
	}
	}

	|VOID NAME S_O S_C M_O 
	{
	if(!O_sa_parse){ 
		if(*$2 != "main"){
			semantic_error();
		}

	 	if(is_def){
	 		// if definition of main exists then check 
	 		//  if the dtype of both the paramter list matches
	 		if(!( param_dt1 == param_dt2)){
	 			semantic_error();
	 		}
	 		// no need to insert in param symtable as there is no param listss
	 	}
	}
	}
	var_declr_list stat_list M_C
	|VOID NAME S_O param_list S_C M_O 
	{
	if(!O_sa_parse){ 
		if(*$2 != "main"){
			semantic_error();
		}

		if(is_def){
			if(!( param_dt1 == param_dt2)){
				semantic_error();
			}
			else{
				//  now insert the variable in symbol table
				for(int i=0; i<param_dt2.size(); i++){
					symbol_TableO.insert_P(param_var2[i], param_dt2[i]);
				}
			}
		}
		else{
			//  now insert the variable in symbol table
			for(int i=0; i<param_dt1.size(); i++){
				symbol_TableO.insert_P(param_var1[i], param_dt1[i]);
			}
		}
	}
	} 
	var_declr_list stat_list M_C 
	;

	
param_list: dtype NAME 
	{
	if(!O_sa_parse){ 
		if(is_def){
			param_dt2.push_back($1);
			param_var2.push_back($2);
		}
		else{
			param_dt1.push_back($1);
			param_var1.push_back($2);
		}
	}
	}
   |param_list COMMA dtype NAME
   {
	if(!O_sa_parse){ 
		if(is_def){
			param_dt2.push_back($3);
			param_var2.push_back($4);
		}
		else{
			param_dt1.push_back($3);
			param_var1.push_back($4);
		}
	}
	}
   ;

var_declr_list: | var_declr_list var_declr ;

var_declr: dtype varlist SCOLON 
	{
	if(!O_sa_parse){ 
		for(auto v:var_vec){
			symbol_TableO.insert_L(v, $1);
		}
		var_vec.clear();
	}
	}; 

stat_list: |stat_list WRITE wrt SCOLON
           |stat_list READ rd SCOLON
	   |stat_list NAME ASSIGN_OP exp SCOLON
	{
	if(!O_sa_parse){ 
		SymTablePtr itsptr = symbol_TableO.findVar($2);

		Name_Ast* leaf_node = new Name_Ast(itsptr->second,Name_E,itsptr);

		Asgn_Ast* newnode       = new Asgn_Ast(leaf_node->dtype, Asgn_E, leaf_node, $4);
		(program_root->Ast_List).push_back(newnode);
	}
	};
	
varlist: varlist COMMA NAME  
	{
	if(!O_sa_parse){ 
		var_vec.push_back($3);
	}
	} 
	|NAME 
	{
	if(!O_sa_parse){ 
		var_vec.push_back($1);
	}
	};

rd: NAME 
	{
	if(!O_sa_parse){ 
		SymTablePtr itsptr = symbol_TableO.findVar($1);

		Name_Ast* leaf_node = new Name_Ast(itsptr->second,Name_E,itsptr);
		Io_Ast* newnode     = new Io_Ast(leaf_node->dtype ,READ_E, leaf_node);

		(program_root->Ast_List).push_back(newnode);
	}
	}
	;
wrt: NAME {
	if(!O_sa_parse){ 
		SymTablePtr itsptr = symbol_TableO.findVar($1);

		Name_Ast* leaf_node = new Name_Ast(itsptr->second,Name_E,itsptr);
		Io_Ast* newnode     = new Io_Ast(leaf_node->dtype, PRINT_E, leaf_node);

		(program_root->Ast_List).push_back(newnode);
	}}
	| NUM {
	if(!O_sa_parse){ 
		Int_Ast* leaf_node = new Int_Ast(INTe,Num_E,$1);
		Io_Ast* newnode     = new Io_Ast(INTe, PRINT_E, leaf_node);
		(program_root->Ast_List).push_back(newnode);
	}}
	| FNUM {
	if(!O_sa_parse){ 
		Float_Ast* leaf_node = new Float_Ast(FLOATe,Num_E,$1);
		Io_Ast* newnode     = new Io_Ast(FLOATe, PRINT_E, leaf_node);
		(program_root->Ast_List).push_back(newnode);
	}}
	| STR_CONST {
	if(!O_sa_parse){ 
		Str_Ast* leaf_node = new Str_Ast(STRINGe,String_E,*$1);
		Io_Ast* newnode     = new Io_Ast(STRINGe, PRINT_E, leaf_node);
		(program_root->Ast_List).push_back(newnode);
	}}
	;

exp:  NAME {
	if(!O_sa_parse){ 
		SymTablePtr itsptr = symbol_TableO.findVar($1);

		$$ = new Name_Ast(itsptr->second,Name_E,itsptr);
	}}
	| NUM  {
	if(!O_sa_parse){ 
		$$ = new Int_Ast(INTe,Num_E,$1);
	}}
	| FNUM {
	if(!O_sa_parse){ 
		$$ = new Float_Ast(FLOATe,Num_E,$1);
	}}
	| STR_CONST {
	if(!O_sa_parse){ 
		$$ = new Str_Ast(STRINGe,String_E,*$1);
	}}
	| exp PLUS exp{
	if(!O_sa_parse){ 
		$$ = new Arith_Ast($1->dtype,Plus_E, $1, $3);
	}}	
	| exp MINUS exp {
	if(!O_sa_parse){ 
		$$ = new Arith_Ast($1->dtype,Minus_E, $1, $3);
	}}
	| exp MULT exp {
	if(!O_sa_parse){ 
		$$ = new Arith_Ast($1->dtype,Mult_E, $1, $3);
	}}
	| exp DIV exp {
	if(!O_sa_parse){ 
		$$ = new Arith_Ast($1->dtype,Div_E, $1, $3);
	}}
	| S_O exp S_C {
	if(!O_sa_parse){ 
		$$ = $2;
	}}
	| MINUS exp %prec UMINUS{
	if(!O_sa_parse){ 
		$$ = new Arith_Ast($2->dtype,Uminus_E, $2, NULL);
	}}
	| exp QUES exp COLON exp{
	if(!O_sa_parse){ 
		$$ = new Qmark_Ast($3->dtype,Q_Col_E, $1,$3,$5);
	}}
	| exp GREATER_THAN_EQUAL exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,GTE_E, $1, $3);
	}}
	| exp GREATER_THAN exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,GT_E, $1, $3);
	}}
	| exp LESS_THAN exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,LT_E, $1, $3);
	}}
	| exp LESS_THAN_EQUAL exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,LTE_E, $1, $3);
	}}
	| exp OR exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,Or_E, $1, $3);
	}}
	| exp AND exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,And_E, $1, $3);
	}}
	| exp EQUAL exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,EQ_E, $1, $3);
	}}
	| exp NOT_EQUAL exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,NEQ_E, $1, $3);
	}}
	| NOT exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,Not_E, $2, NULL);
	}}
	;
dtype: INTEGER 
	{
	if(!O_sa_parse){ 
		$$=INTe;
	}}
	| FLOAT    
	{
	if(!O_sa_parse){ 
		$$=FLOATe;
	}}
	| BOOL     
	{
	if(!O_sa_parse){ 
		$$=BOOLe;
	}}
	| STRING   
	{
	if(!O_sa_parse){ 
		$$=STRINGe;
	}};

%%


/////////////////////////////////////////////////////
int main(int argc, char const *argv[]){
	char input_file[256];
	char tok_file[256];
	char ast_file[256];


	for(int i=1; i<argc; i++){
		if(argv[i][0] != '-'){
			strcpy(input_file, argv[i]);
		}
		else if(strcmp(argv[i], "-tokens") == 0){ O_tokens = true; }
		else if(strcmp(argv[i], "-ast") == 0)    { O_ast = true; }
		else if(strcmp(argv[i], "-sa-scan") == 0){ O_sa_scan = true; }
		else if(strcmp(argv[i], "-sa-parse") == 0){ O_sa_parse = true; }
		else{}
	}

	strcpy(tok_file, input_file);
	strcpy(ast_file, input_file);

	strcat(tok_file, ".toks");
	strcat(ast_file, ".ast");


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
		ofstream fout;
		fout.open(string(ast_file), ios::trunc);
		
		program_root->printTree(fout);
		fout.close();
	}

	return 0;
}

/////////////////////////////////////////////////////
void yyerror(const char *mesg){
	fprintf(stderr,"%s\n",mesg);
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
