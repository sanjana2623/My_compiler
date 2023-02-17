#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "sclp.h"
using namespace std;

// external variable ###########################################
extern void semantic_error();
extern SymTable global_sym_table;

// Now class declaration ###########################################
// forward declaration
class Exp_Ast;
class Fun_def;
typedef map <string, Fun_def*> FunTable;
typedef FunTable::iterator FunTablePtr;

extern FunTable fun_table;
// #####################Helper Class##############################
// this class is for holding parameter list

class Param_class{    //eg.: fun(int a, float b)
public:
	vector<DataType> dtype;  // for dtype of parameter list
	vector<string*>  var;  // for var name of parameter list

	void printAst(ostream& aout){ 
		for(int i=0; i<(this->dtype).size(); i++){
			aout<<"\n"<< *((this->var)[i]) << "_  Type:";
			printEnumDT(aout, (this->dtype)[i]);
		}
	}
};

class VarList_class{      //eg.: int x,y,z;
public:	
	DataType dtype;
	vector<string*> var_list;  // for varlist
};

class ArgList_class{    // eg.: fun(x,5,5.5,"ok")
public:	
	vector<Exp_Ast*> arg_list;

	void printAst(ostream& aout);
};

// extern Param_class* PC_declr; //for holding param list (declr)

// #####################Helper END##########################


// #####################for TAC##############################
// i am defining it here beacuse it us going to be 
// used by Ast_Node class

//base class for TAC
class Tac_Node{
public:
	virtual void printTac(ostream& tout){}
};

// type of leaf node in tac 
class Leaf_Tac : public Tac_Node{
public:
	void printTac(ostream& tout){}
	virtual string fn_data(){} 
};

// #####################for TAC END##########################


// #####################for AST##############################
//base class
class Ast_Node{
public:
	DataType dtype;
	NodeText label;
	virtual void printAst(ostream& aout){}
	virtual Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list){}
};


// helper class for holding list of AST NODE
class Ast_List_class{
public:
	vector<Ast_Node*> Ast_List;

	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};


//derived classes, for different type of node
class Exp_Ast : public Ast_Node{
public:
	void printAst(ostream& aout){}
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list){}
};

// type of exp 
class Unary_Exp : public Exp_Ast{
public:
	void printAst(ostream& aout){}
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list){}
};

class Bin_Exp : public Exp_Ast{
public:
	void printAst(ostream& aout){}
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list){}
};

class Tri_Exp : public Exp_Ast{
public:
	void printAst(ostream& aout){}
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list){}
};

// different types of node of exp
// unary exp (leaf node) //////////////////////////////////////////////////////
class Name_Ast : public Unary_Exp{
public:
	SymTablePtr data;

	Name_Ast(DataType dtype, NodeText label, SymTablePtr data);
	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};


class Int_Ast : public Unary_Exp{
public:
	int data;

	Int_Ast(DataType dtype, NodeText label, int data);
	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};


class Float_Ast : public Unary_Exp{
public:
	float data;

	Float_Ast(DataType dtype, NodeText label, float data);
	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};

class Str_Ast : public Unary_Exp{
public:
	string data;

	Str_Ast(DataType dtype, NodeText label, string data);
	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};

class Io_Ast : public Unary_Exp{
public:
	Exp_Ast* data;

	Io_Ast(DataType dtype, NodeText label, Exp_Ast* data);
	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};

// binary exp //////////////////////////////////////////////////////
class Asgn_Ast : public Bin_Exp{
public:
	Name_Ast* LHS;
	Ast_Node* RHS;

	Asgn_Ast(DataType dtype, NodeText label, Name_Ast* LHS, Ast_Node* RHS);
	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};


class Arith_Ast : public Bin_Exp{
public:
	Exp_Ast* LHS;
	Exp_Ast* RHS;

	Arith_Ast(DataType dtype, NodeText label, Exp_Ast* LHS, Exp_Ast* RHS);
	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};


class Relat_Ast : public Bin_Exp{
public:
	Exp_Ast* LHS;
	Exp_Ast* RHS;

	Relat_Ast(DataType dtype, NodeText label, Exp_Ast* LHS, Exp_Ast* RHS);
	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};

// ternary exp //////////////////////////////////////////////////////
class Qmark_Ast : public Tri_Exp{
public:
	Exp_Ast* Cond_part;
	Exp_Ast* True_Part;
	Exp_Ast* False_Part;
	int stemp_index;

	Qmark_Ast(DataType dtype, NodeText label, 
		Exp_Ast* cp, Exp_Ast* tp, Exp_Ast* fp, int si);
	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};

//  while & do while loop node
class While_Ast : public Ast_Node{
public:
	Exp_Ast* cond_part;
	Ast_List_class* body_part;

	While_Ast(DataType dtype, NodeText label,
			  Exp_Ast* cond_part,Ast_List_class* body_part);
	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};

//  if else loop node
class If_Else_Ast : public Ast_Node{
public:
	Exp_Ast* cond_part;
	Ast_List_class* true_Part;
	Ast_List_class* false_Part;

	If_Else_Ast(DataType dtype, NodeText label,
			  Exp_Ast* cond_part,
			  Ast_List_class* true_Part,Ast_List_class* false_Part);
	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};

class Return_Ast : public Ast_Node{
public:
	Exp_Ast* data;

	Return_Ast(NodeText label, Exp_Ast* data);
	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};

class Fun_call_Ast : public Ast_Node{
public:
	Fun_def* fun;
	ArgList_class* arglist;

	Fun_call_Ast(NodeText label,Fun_def* fun, ArgList_class* arglist);
	void printAst(ostream& aout);
	Leaf_Tac* insertTac(vector<Tac_Node*> &Tac_list);
};

// #####################for Function##############################
class Fun_def{
public:
	bool has_return_stmt;
	bool is_declr;         // is fun declare already?
	string fname;          // fun name
	DataType rtype;        // return type
	Param_class* param_L;  // list of parameter and their dtype	
	SymTable param_T;
	SymTable local_T;
	Ast_List_class* body_part;
	vector<Tac_Node*> Tac_list;

	Fun_def(string fname, DataType rtype, bool is_declr,Param_class* plc){
		this->has_return_stmt = false;
		this->fname = fname;
		this->rtype = rtype;
		this->is_declr = is_declr;
		this->param_L = plc;
	}
	void make_param_Stable(){
		for(int i=0; i<(this->param_L->dtype).size(); i++){
			this->insert_P((this->param_L->var)[i],(this->param_L->dtype)[i]);
		}
	}

	void insert_P(string *var, DataType dtype){
		if(this->param_T.insert( {*var, dtype} ).second){
			// inserted. all good
		}
		else{
			// already key exists
			semantic_error();
		}
	}
	void insert_L(string *var, DataType dtype){
		//  also check in paramter table
		if((this->param_T).find(*var)==(this->param_T).end()){
			if(this->local_T.insert( {*var, dtype} ).second){
				// inserted. all good
			}
			else{
				// already key exists
				semantic_error();
			}
		}
		else{
			// same var found in paramter list
			semantic_error();
		}
	}
	SymTablePtr findVar(string *var){

		SymTablePtr itsptr = (this->local_T).find(*var);
		if(itsptr==(this->local_T).end()){

			itsptr = (this->param_T).find(*var);
			if(itsptr==(this->param_T).end()){

				itsptr = global_sym_table.find(*var);
				if(itsptr==global_sym_table.end()){
					// handle error now
					semantic_error();
				}
			}
		}
		return itsptr;
	}

	void printAst(ostream& aout);

};
// #####################for Function END##########################


//////////////////////////////////////////////////
// class Symbol_Table{
// public:
// 	SymTable global_T;
// 	// SymTable param_T;
// 	// SymTable local_T;

// 	void insert_G(string *var, DataType dtype){
// 		if(this->global_T.insert( {*var, dtype} ).second){
// 			// inserted. all good
// 		}
// 		else{
// 			// already key exists
// 			semantic_error();
// 		}
// 	}
	// void insert_P(string *var, DataType dtype){
	// 	if(this->param_T.insert( {*var, dtype} ).second){
	// 		// inserted. all good
	// 	}
	// 	else{
	// 		// already key exists
	// 		semantic_error();
	// 	}
	// }
	// void insert_L(string *var, DataType dtype){
	// 	//  also check in paramter table
	// 	if((this->param_T).find(*var)==(this->param_T).end()){
	// 		if(this->local_T.insert( {*var, dtype} ).second){
	// 			// inserted. all good
	// 		}
	// 		else{
	// 			// already key exists
	// 			semantic_error();
	// 		}
	// 	}
	// 	else{
	// 		// same var found in paramter list
	// 		semantic_error();
	// 	}
	// }

// 	SymTablePtr findVar(string *var){

// 		SymTablePtr itsptr = (this->local_T).find(*var);
// 		if(itsptr==(this->local_T).end()){

// 			itsptr = (this->param_T).find(*var);
// 			if(itsptr==(this->param_T).end()){

// 				itsptr = (this->global_T).find(*var);
// 				if(itsptr==(this->global_T).end()){
// 					// handle error now
// 					semantic_error();
// 					return itsptr;
// 				}
// 			}
// 		}
// 		return itsptr;
// 	}
// };
//////////////////////////////////////////////////


// program node IMP ///////////////////////////////
class Procedure_Node{
public:	
	vector<Fun_def*> fun_list;
	void printTree(ostream& aout){
		aout<<"Program:";
		for(auto f: fun_table){
			(f.second)->printAst(aout);
		}
		aout<<"\n";
	}
	void makeTac(ostream& tout){
		cout<<"prijting tac\n";
	}
};



// class Program_Node{
// public:
// 	vector<Ast_Node*> Ast_List;
// 	vector<Tac_Node*> Tac_list;

// 	// #####################for AST##############################

// 	void printTree(ostream& aout){

// 		aout<<"Program:\n**PROCEDURE: main\nReturn Type: <void>\n\
// Formal Parameters:";

// 		// print paramter list of main 
// 		PC_declr->printAst(aout);

// 		aout<<"\n**BEGIN: Abstract Syntax Tree";

// 		for(auto x:this->Ast_List){
// 			x->printAst(aout);
// 		}

// 		aout<<"\n**END: Abstract Syntax Tree\n";
// 	}


// 	// #####################for TAC##############################
// 	void makeTac(ostream& tout){
// 		for(auto x:this->Ast_List){
// 			x->insertTac(this->Tac_list);
// 		}

// 		if(! (this->Tac_list).empty() ){ 
// 			tout<<"**PROCEDURE: main\n**BEGIN: Three Address Code Statements\n";
// 		}
// 		for(auto x:this->Tac_list){
// 			x->printTac(tout);
// 		}

// 		if(! (this->Tac_list).empty() ){ 
// 			tout<<"**END: Three Address Code Statements\n";
// 		}
// 	}
// };
/////////////////////////////////////////////////