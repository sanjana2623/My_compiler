#include<iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;


// ENUM ######################################################
enum DataType {INTe, FLOATe, BOOLe, STRINGe, VOIDe};

enum NodeText {Program_E, Asgn_E, Num_E, Name_E, String_E, 
   			   Plus_E, Minus_E, Mult_E, Div_E, Uminus_E, 
   			   GT_E, GTE_E, LT_E, LTE_E, Or_E, And_E, EQ_E, NEQ_E, Not_E, 
   			   Q_Col_E,
   			   PRINT_E, READ_E,
   			   WHILE_E, DO_WHILE_E,
   			   IF_E,
   			   FUNC_E, RETURN_E};

typedef map <string, DataType> SymTable;
typedef SymTable::iterator SymTablePtr;

//  functions to print TOKENS ######################################
void printToken(const char* toksname, char* yytext,int yylineno);

//  functions to print ENUM ######################################
void printEnumDT(ostream& fout, DataType dtype);
void printEnumIO(ostream& fout,NodeText label);
void printEnumArith(ostream& fout,NodeText label);
void printEnumRelat(ostream& fout,NodeText label);
void printEnumOp(ostream& fout,NodeText label);
