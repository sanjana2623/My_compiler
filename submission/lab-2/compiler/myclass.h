#include <fstream>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;

extern void semantic_error();

enum DataType {INTe, FLOATe, BOOLe, STRINGe};

enum NodeText {Program_E, Asgn_E, Num_E, Name_E, String_E, 
   			   Plus_E, Minus_E, Mult_E, Div_E, Uminus_E, 
   			   GT_E, GTE_E, LT_E, LTE_E, Or_E, And_E, EQ_E, NEQ_E, Not_E, 
   			   Q_Col_E,
   			   PRINT_E, READ_E};


typedef map <string, DataType> SymTable;
typedef SymTable::iterator SymTablePtr;

extern vector<string*>  param_var1;  // for var name of parameter list
extern vector<string*>  param_var2; 
extern vector<DataType> param_dt1;  // for dtype of parameter list
extern vector<DataType> param_dt2; 
extern bool is_def;


//base class
class Ast_Node{
public:
	DataType dtype;
	NodeText label;
	virtual void printMe(ostream& fout){}
};

//derived classes, for different type of node
class Exp_Ast : public Ast_Node{
public:
	void printMe(ostream& fout){}

};

// type of exp 
class Unary_Exp : public Exp_Ast{
public:
	void printMe(ostream& fout){}

};
class Bin_Exp : public Exp_Ast{
public:
	void printMe(ostream& fout){}

};
class Tri_Exp : public Exp_Ast{
public:
	void printMe(ostream& fout){}

};

// different types of node of exp
// unary exp (leaf node) //////////////////////////////////////////////////////
class Name_Ast : public Unary_Exp{
public:
	SymTablePtr data;

	Name_Ast(DataType dtype, NodeText label, SymTablePtr data){
		this->dtype = dtype;
		this->label = label;
		this->data = data;
	}
	void printMe(ostream& fout){
		fout<<"Name : "<<this->data->first<<"_";

		switch(this->data->second){
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
};

class Int_Ast : public Unary_Exp{
public:
	int data;

	Int_Ast(DataType dtype, NodeText label, int data){
		this->dtype = dtype;
		this->label = label;
		this->data = data;
	}
	void printMe(ostream& fout){
		fout<<"Num : "<<this->data<<"<int>";
	}
};

class Float_Ast : public Unary_Exp{
public:
	float data;

	Float_Ast(DataType dtype, NodeText label, float data){
		this->dtype = dtype;
		this->label = label;
		this->data = data;
	}
	void printMe(ostream& fout){
		fout<<"Num : "<<fixed<<setprecision(2)<<this->data<<"<float>";
	}
};

class Str_Ast : public Unary_Exp{
public:
	string data;

	Str_Ast(DataType dtype, NodeText label, string data){
		this->dtype = dtype;
		this->label = label;
		this->data = data;
	}
	void printMe(ostream& fout){
		fout<<"String : "<<this->data<<"<string>";
	}
};

class Io_Ast : public Unary_Exp{
public:
	Exp_Ast* data;

	Io_Ast(DataType dtype, NodeText label, Exp_Ast* data){
		if(label == READ_E){
			if(data->dtype == STRINGe || data->dtype == BOOLe){
				semantic_error();
			}
		}
		else if(label == PRINT_E){
			if(data->dtype == BOOLe){
				semantic_error();
			}
		}
		else{}


		this->dtype = dtype;
		this->label = label;
		this->data = data;
	}
	void printMe(ostream& fout){
		switch(this->label){
			case READ_E : {
				fout<<"Read: ";
				break;
			}
			case PRINT_E : {
				fout<<"Write: ";
				break;
			}
		}
		data->printMe(fout);
		fout<<"\n";
	}
};



// binary exp //////////////////////////////////////////////////////
class Asgn_Ast : public Bin_Exp{
public:
	Name_Ast* LHS;
	Exp_Ast* RHS;

	Asgn_Ast(DataType dtype, NodeText label, Name_Ast* LHS, Exp_Ast* RHS){
		if (LHS->dtype != RHS->dtype){ 
			semantic_error();
		}
		this->dtype = dtype;
		this->label = label;
		this->LHS = LHS;
		this->RHS = RHS;
	}
	void printMe(ostream& fout){
		fout<<"Asgn:\n";
		
		fout<<"LHS (";
		this->LHS->printMe(fout);
		fout<<")\n";

		fout<<"RHS (";
		this->RHS->printMe(fout);
		fout<<")";

		fout<<"\n";
	}
};

class Arith_Ast : public Bin_Exp{
public:
	Exp_Ast* LHS;
	Exp_Ast* RHS;

	Arith_Ast(DataType dtype, NodeText label, Exp_Ast* LHS, Exp_Ast* RHS){
		if(RHS != NULL){ 
			if (LHS->dtype != RHS->dtype){ 
				semantic_error();
			}
		}
		if(LHS->dtype == STRINGe || LHS->dtype == BOOLe){
			semantic_error();
		}

		this->dtype = dtype;
		this->label = label;
		this->LHS = LHS;
		this->RHS = RHS;
	}
	void printMe(ostream& fout){
		fout<<"\nArith: ";

		switch(this->label){
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

		switch(this->dtype){
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
		
		fout<<"\nL_Opd (";
		this->LHS->printMe(fout);
		fout<<")";

		if(this->RHS != NULL){ 
			fout<<"\nR_Opd (";
			this->RHS->printMe(fout);
			fout<<")";
		}
	}
};

class Relat_Ast : public Bin_Exp{
public:
	Exp_Ast* LHS;
	Exp_Ast* RHS;

	Relat_Ast(DataType dtype, NodeText label, Exp_Ast* LHS, Exp_Ast* RHS){
		if(RHS != NULL){ 
			if (LHS->dtype != RHS->dtype){ 
				semantic_error();
			}
		} 
		else{}

		if(label == Or_E || label == And_E || label == Not_E){
			if (LHS->dtype != BOOLe){
				semantic_error();
			}
		}
		else{
			if(LHS->dtype == STRINGe || LHS->dtype == BOOLe){
				semantic_error();
			}
		}


		this->dtype = dtype;
		this->label = label;
		this->LHS = LHS;
		this->RHS = RHS;
	}
	void printMe(ostream& fout){		
		fout<<"\nCondition: ";
		switch(this->label){
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

		fout<<"\nL_Opd (";
		this->LHS->printMe(fout);
		fout<<")";

		if(this->RHS != NULL){ 
			fout<<"\nR_Opd (";
			this->RHS->printMe(fout);
			fout<<")";
		}
	}
};

// ternary exp //////////////////////////////////////////////////////
class Qmark_Ast : public Tri_Exp{
public:
	Exp_Ast* Cond_part;
	Exp_Ast* True_Part;
	Exp_Ast* False_Part;

	Qmark_Ast(DataType dtype, NodeText label, Exp_Ast* cp, Exp_Ast* tp, Exp_Ast* fp){

		if (tp->dtype != fp->dtype){ 
			semantic_error();
		}
		if (cp->dtype != BOOLe){ 
			semantic_error();
		}

		this->dtype = dtype;
		this->label = label;
		this->Cond_part = cp;
		this->True_Part = tp;
		this->False_Part = fp;
	}
	void printMe(ostream& fout){		
		this->Cond_part->printMe(fout);

		fout<<"\nTrue_Part (";
		this->True_Part->printMe(fout);
		fout<<")\n";

		fout<<"False_Part (";
		this->False_Part->printMe(fout);
		fout<<")";
	}
};


//////////////////////////////////////////////////
class Symbol_Table{
public:
	SymTable global_T;
	SymTable param_T;
	SymTable local_T;

	void insert_G(string *var, DataType dtype){
		if(this->global_T.insert( {*var, dtype} ).second){
			// inserted. all good
		}
		else{
			// already key exists
			semantic_error();
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

				itsptr = (this->global_T).find(*var);
				if(itsptr==(this->global_T).end()){
					// handle error now
					semantic_error();
					return itsptr;
				}
			}
		}
		return itsptr;
	}

};
//////////////////////////////////////////////////


// program node IMP ///////////////////////////////
class Program_Node{
public:
	vector<Ast_Node*> Ast_List;

	void printTree(ostream& fout){

	fout<<"Program:\n**PROCEDURE: main\n\tReturn Type: <void>\n\t\
Formal Parameters:\n";

	if(is_def){  ///////////////////////////////////////////////////////
		for(int i=0; i<param_dt2.size(); i++){
			fout<< *param_var2[i] << "_  Type:";

			switch(param_dt2[i]){
				case INTe : {
					fout<<"<int>\n";
					break;
				}
				case FLOATe : {
					fout<<"<float>\n";
					break;
				}
				case BOOLe : {
					fout<<"<bool>\n";
					break;
				}
				case STRINGe : {
					fout<<"<string>\n";
					break;
				}
			}
		}
	}
	else{  ////////////////////////////////////////////////////////////
		for(int i=0; i<param_dt1.size(); i++){
			fout<< *param_var1[i] << "_  Type:";

			switch(param_dt1[i]){
				case INTe : {
					fout<<"<int>\n";
					break;
				}
				case FLOATe : {
					fout<<"<float>\n";
					break;
				}
				case BOOLe : {
					fout<<"<bool>\n";
					break;
				}
				case STRINGe : {
					fout<<"<string>\n";
					break;
				}
			}
		}
	}

	/////////////////////////////////////////////////
	fout<<"**BEGIN: Abstract Syntax Tree\n";

		for(auto x:this->Ast_List){
			x->printMe(fout);
		}

		fout<<"**END: Abstract Syntax Tree\n";
	}

};
/////////////////////////////////////////////////
