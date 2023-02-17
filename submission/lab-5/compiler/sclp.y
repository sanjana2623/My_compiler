%{
#include <string>
#include <vector>
#include "ast.h"    
using namespace std;

int yylex();
void yyerror(const char *mesg);
void semantic_error();

// external variable ###########################################
extern Program_Node* program_root; //IMP for holding list of AST
extern Param_class* PC_def; //for holding param list (def)
extern Param_class* PC_declr; //for holding param list (declr)


extern bool O_sa_parse;
extern bool is_def;
extern int stemp_index;

// global variable #############################################
vector<string*> var_vec;  // for varlist

Symbol_Table symbol_TableO;       // symbol table object consist of all sym table

%}

%union{
	int num;
	double fnum;
	string * name;
	enum DataType dtype;
	class Ast_Node* ast_Node;
	class Exp_Ast* exp_Ast;
	class Io_Ast* io_Ast;
	class If_Else_Ast* if_Else_Ast;
	class Ast_List_class* ast_List;
	class Param_class* param_class;
}

%start program

%token IF ELSE WHILE DO
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

%nonassoc then
%nonassoc ELSE

%type <dtype> dtype
%type <exp_Ast> exp
%type <io_Ast> wrt
%type <io_Ast> rd
%type <ast_Node> while_block
%type <ast_Node> do_while_block
%type <ast_Node> single_stmnt
%type <ast_List> stat_list
%type <ast_List> else_block
%type <if_Else_Ast> if_block
%type <param_class> param_list
%type <param_class> param_listA

%%
program: glob_declr fun_def proc_def 
	|fun_def glob_declr proc_def  
	|glob_declr fun_def glob_declr proc_def  
	|glob_declr proc_def 
	|fun_def proc_def 
	|proc_def ;

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

fun_def: 
	VOID NAME S_O param_list S_C SCOLON 
	{
	if(!O_sa_parse){ 
		is_def = true;
		if(*$2 != "main"){
			semantic_error();
		}
		PC_def = $4;
	}
	}
	|dtype NAME S_O param_list S_C SCOLON
	{
	if(!O_sa_parse){ 
		semantic_error();
	}
	}
	;


proc_def:  
	dtype NAME S_O param_list S_C M_O var_declr_list stat_list M_C 
	{
	if(!O_sa_parse){ 
		semantic_error();
	}
	}
	|VOID NAME S_O param_list S_C M_O 
	{
	if(!O_sa_parse){ 
		if(*$2 != "main"){
			semantic_error();
		}
		PC_declr = $4;

		if(is_def){
			if(!( PC_def->dtype == PC_declr->dtype)){
				semantic_error();
			}
		}
		//  now insert the variable in symbol table
		for(int i=0; i<(PC_declr->dtype).size(); i++){
			symbol_TableO.insert_P((PC_declr->var)[i], (PC_declr->dtype)[i]);
		}
	}
	} 
	var_declr_list stat_list M_C 
	{
	if(!O_sa_parse){ 
		program_root->Ast_List = $9->Ast_List;
	}
	}
	;


// param list can be empty
param_list:
	{
	if(!O_sa_parse){ 
		$$ = new Param_class();
	}
	}
	|param_listA
	{
	if(!O_sa_parse){ 
		$$=$1;
	}
	}
 ;
	
param_listA: dtype NAME  
	{
	if(!O_sa_parse){ 
		Param_class* PC = new Param_class();
		(PC->dtype).push_back($1);
		(PC->var).push_back($2);

		$$ = PC;
	}
	}
   |param_listA COMMA dtype NAME
   {
	if(!O_sa_parse){ 
		($1->dtype).push_back($3);
		($1->var).push_back($4);

		$$ = $1;
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


stat_list:
	{
	if(!O_sa_parse){ 
		$$ = new Ast_List_class(); //IMP for holding list of AST
	}
	}
	|stat_list single_stmnt
	{
	if(!O_sa_parse){ 
		($1->Ast_List).push_back($2);
		$$ = $1;
	}
	}
	;

single_stmnt:
	WRITE wrt SCOLON
	{
	if(!O_sa_parse){ 
		$$ = $2;
	}
	}

   |READ rd SCOLON
	{
	if(!O_sa_parse){ 
		$$ = $2;
	}
	}
   |NAME ASSIGN_OP exp SCOLON
	{
	if(!O_sa_parse){ 
		SymTablePtr itsptr = symbol_TableO.findVar($1);
		Name_Ast* leaf_node = new Name_Ast(itsptr->second,Name_E,itsptr);
		$$ = new Asgn_Ast(leaf_node->dtype, Asgn_E, leaf_node, $3);
	}
	}
   |while_block 
   {
	if(!O_sa_parse){ 
	$$ = $1;
   }
   }
   |do_while_block 
   {
	if(!O_sa_parse){ 
	$$ = $1;
   }
   }
   |if_block else_block
   {
	if(!O_sa_parse){ 
		($1->false_Part) = $2;
		$$ = $1;
   }
   }
   |if_block %prec then
   {
	if(!O_sa_parse){ 
		$$ = $1;
   }
   }
   ;


while_block:
    WHILE S_O exp S_C M_O stat_list M_C
	{
	if(!O_sa_parse){ 
		$$ = new While_Ast(BOOLe, WHILE_E, $3, $6);
	}
	}
	|WHILE S_O exp S_C single_stmnt
	{
	if(!O_sa_parse){ 
		Ast_List_class* alc = new Ast_List_class();
		(alc->Ast_List).push_back($5);
		$$ = new While_Ast(BOOLe, WHILE_E, $3, alc);
	}
	}
    ;

do_while_block: 
    DO M_O stat_list M_C WHILE S_O exp S_C SCOLON
	{
	if(!O_sa_parse){ 
		$$ = new While_Ast(BOOLe, DO_WHILE_E, $7, $3);
	}
	}
	|DO  single_stmnt  WHILE S_O exp S_C SCOLON
	{
	if(!O_sa_parse){ 
		Ast_List_class* alc = new Ast_List_class();
		(alc->Ast_List).push_back($2);
		$$ = new While_Ast(BOOLe, DO_WHILE_E, $5, alc);
	}
	}
    ;


if_block : 
    IF S_O exp S_C M_O stat_list M_C
	{
	if(!O_sa_parse){ 
		$$ = new If_Else_Ast(BOOLe, IF_E, $3, $6, NULL);
	}
	}
	|IF S_O exp S_C  single_stmnt 
	{
	if(!O_sa_parse){ 
		Ast_List_class* alc = new Ast_List_class();
		(alc->Ast_List).push_back($5);
		$$ = new If_Else_Ast(BOOLe, IF_E, $3, alc, NULL);
	}
	}
	;

else_block : 
    ELSE M_O stat_list M_C
	{
	if(!O_sa_parse){ 
		$$ = $3;
	}
	}
	|ELSE single_stmnt
	{
	if(!O_sa_parse){ 
		Ast_List_class* alc = new Ast_List_class();
		(alc->Ast_List).push_back($2);
		$$ = alc;
	}
	}
	;


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
		$$ = new Io_Ast(leaf_node->dtype ,READ_E, leaf_node);
	}
	}
	;
wrt: NAME {
	if(!O_sa_parse){ 
		SymTablePtr itsptr = symbol_TableO.findVar($1);
		Name_Ast* leaf_node = new Name_Ast(itsptr->second,Name_E,itsptr);
		$$ = new Io_Ast(leaf_node->dtype, PRINT_E, leaf_node);
	}}
	|NUM {
	if(!O_sa_parse){ 
		Int_Ast* leaf_node = new Int_Ast(INTe,Num_E,$1);
		$$ = new Io_Ast(INTe, PRINT_E, leaf_node);
	}}
	|FNUM {
	if(!O_sa_parse){ 
		Float_Ast* leaf_node = new Float_Ast(FLOATe,Num_E,$1);
		$$ = new Io_Ast(FLOATe, PRINT_E, leaf_node);
	}}
	|STR_CONST {
	if(!O_sa_parse){ 
		Str_Ast* leaf_node = new Str_Ast(STRINGe,String_E,*$1);
		$$ = new Io_Ast(STRINGe, PRINT_E, leaf_node);
	}}
	;

exp:  NAME {
	if(!O_sa_parse){ 
		SymTablePtr itsptr = symbol_TableO.findVar($1);

		$$ = new Name_Ast(itsptr->second,Name_E,itsptr);
	}}
	|NUM  {
	if(!O_sa_parse){ 
		$$ = new Int_Ast(INTe,Num_E,$1);
	}}
	|FNUM {
	if(!O_sa_parse){ 
		$$ = new Float_Ast(FLOATe,Num_E,$1);
	}}
	|STR_CONST {
	if(!O_sa_parse){ 
		$$ = new Str_Ast(STRINGe,String_E,*$1);
	}}
	|exp PLUS exp{
	if(!O_sa_parse){ 
		$$ = new Arith_Ast($1->dtype,Plus_E, $1, $3);
	}}  
	|exp MINUS exp {
	if(!O_sa_parse){ 
		$$ = new Arith_Ast($1->dtype,Minus_E, $1, $3);
	}}
	|exp MULT exp {
	if(!O_sa_parse){ 
		$$ = new Arith_Ast($1->dtype,Mult_E, $1, $3);
	}}
	|exp DIV exp {
	if(!O_sa_parse){ 
		$$ = new Arith_Ast($1->dtype,Div_E, $1, $3);
	}}
	|S_O exp S_C {
	if(!O_sa_parse){ 
		$$ = $2;
	}}
	|MINUS exp %prec UMINUS{
	if(!O_sa_parse){ 
		$$ = new Arith_Ast($2->dtype,Uminus_E, $2, NULL);
	}}
	|exp QUES exp COLON exp{
	if(!O_sa_parse){ 
		stemp_index++;
		$$ = new Qmark_Ast($3->dtype,Q_Col_E, $1,$3,$5,stemp_index);
	}}
	|exp GREATER_THAN_EQUAL exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,GTE_E, $1, $3);
	}}
	|exp GREATER_THAN exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,GT_E, $1, $3);
	}}
	|exp LESS_THAN exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,LT_E, $1, $3);
	}}
	|exp LESS_THAN_EQUAL exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,LTE_E, $1, $3);
	}}
	|exp OR exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,Or_E, $1, $3);
	}}
	|exp AND exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,And_E, $1, $3);
	}}
	|exp EQUAL exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,EQ_E, $1, $3);
	}}
	|exp NOT_EQUAL exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,NEQ_E, $1, $3);
	}}
	|NOT exp{
	if(!O_sa_parse){ 
		$$ = new Relat_Ast(BOOLe ,Not_E, $2, NULL);
	}}
	;

dtype: INTEGER 
	{
	if(!O_sa_parse){ 
		$$=INTe;
	}}
	|FLOAT    
	{
	if(!O_sa_parse){ 
		$$=FLOATe;
	}}
	|BOOL     
	{
	if(!O_sa_parse){ 
		$$=BOOLe;
	}}
	|STRING   
	{
	if(!O_sa_parse){ 
		$$=STRINGe;
	}};

%%
