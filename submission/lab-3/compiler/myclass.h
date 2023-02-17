#include <fstream>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;

extern void semantic_error();

enum DataType {INTe, FLOATe, BOOLe, STRINGe};

enum NodeText {Program_E, Asgn_E, Num_E, Name_E, String_E, 
   			   Plus_E, Minus_E, Mult_E, Div_E, Uminus_E, 
   			   GT_E, GTE_E, LT_E, LTE_E, Or_E, And_E, EQ_E, NEQ_E, 				   Not_E, 
   			   Q_Col_E,
   			   PRINT_E, READ_E};


typedef map <string, DataType> SymTable;
typedef SymTable::iterator SymTablePtr;

extern vector<string*>  param_var1;  // for var name of parameter list
extern vector<string*>  param_var2; 
extern vector<DataType> param_dt1;  // for dtype of parameter list
extern vector<DataType> param_dt2; 
extern bool b_print; //has begin been printed
extern bool any_tac; //if there are any tac stmt
extern bool is_def;
extern bool silent_ter;
extern bool silent; //should not print anything
extern bool silent_fal;
extern bool for_fal;
extern int t;				//for temp
extern int t_h;			//for helping temp
extern int s_h;				//for stemp
extern int s;	
extern int l;				//for label
extern int l_h;
//base class
class Ast_Node{
public:
	DataType dtype;
	NodeText label;
	int index;
  	bool is_ter;
  	int store;
  	int load;
  	int not_t;
	virtual void printAst(ostream& aout){}
	virtual void printTac(ostream& tout){}
};

//derived classes, for different type of node
class Exp_Ast : public Ast_Node{
public:
	void printAst(ostream& aout){}
	void printTac(ostream& tout){}

};

// type of exp 
class Unary_Exp : public Exp_Ast{
public:
	void printAst(ostream& aout){}
	void printTac(ostream& tout){}

};
class Bin_Exp : public Exp_Ast{
public:
	void printAst(ostream& aout){}
	void printTac(ostream& tout){}

};
class Tri_Exp : public Exp_Ast{
public:
	void printAst(ostream& aout){}
	void printTac(ostream& tout){}

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
		this->index = -1;
		this->is_ter = false;
	}
	
	
	void printAst(ostream& aout){
		aout<<"Name : "<<this->data->first<<"_";
		
		switch(this->data->second){
			case INTe : {
				aout<<"<int>";
				break;
			}
			case FLOATe : {
				aout<<"<float>";
				break;
			}
			case BOOLe : {
				aout<<"<bool>";
				break;
			}
			case STRINGe : {
				aout<<"<string>";
				break;
			}
		}
	}
	
	//------------------------------------------------//
	void printTac(ostream& tout){
		if(!silent){
			tout<<this->data->first<<"_";
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
		this->index = -1;
    	this->is_ter = false;
	}
	void printAst(ostream& aout){
		aout<<"Num : "<<this->data<<"<int>";
	}

	//------------------------------------------------//
	void printTac(ostream& tout){
		if(!silent){
			tout<<this->data;
		}
	}
		
};

class Float_Ast : public Unary_Exp{
public:
	float data;

	Float_Ast(DataType dtype, NodeText label, float data){
		this->dtype = dtype;
		this->label = label;
		this->data = data;
		this->index = -1;
		this->is_ter = false;
	}
	void printAst(ostream& aout){
		aout<<"Num : "<<fixed<<setprecision(2)<<this->data<<"<float>";
	}

	//------------------------------------------------//
	void printTac(ostream& tout){
		if(!silent){
			tout<<fixed<<setprecision(2)<<this->data;
		}
	}
		
};

class Str_Ast : public Unary_Exp{
public:
	string data;

	Str_Ast(DataType dtype, NodeText label, string data){
		this->dtype = dtype;
		this->label = label;
		this->data = data;
		this->index = -1;
		this->is_ter = false;
	}
	void printAst(ostream& aout){
		aout<<"String : "<<this->data<<"<string>";
	}

	//------------------------------------------------//
	void printTac(ostream& tout){
		if(!silent){
			tout<<this->data;
		}
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
		this->index = -1;
		this->is_ter = false;
	}
	void printAst(ostream& aout){
		switch(this->label){
			case READ_E : {
				aout<<"Read: ";
				break;
			}
			case PRINT_E : {
				aout<<"Write: ";
				break;
			}
		}
		data->printAst(aout);
		aout<<"\n";

	}

	//------------------------------------------------//
	void printTac(ostream& tout){
		if(!b_print){
			tout<<"**PROCEDURE: main\n**BEGIN: Three Address Code Statements\n";
			b_print = true;
		}
		if(!silent){
			switch(this->label){
				case READ_E : {
					tout<<"\tread  ";
					break;
				}
				case PRINT_E : {
					tout<<"\twrite  ";
					break;
				}
			}
			data->printTac(tout);
			tout<<"\n";
		}
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
	/*ostream& operator<<(ostream& tout){	
				tout<<t;	
			}*/
	void printAst(ostream& aout){
		aout<<"Asgn:\n";
		
		aout<<"LHS (";
		this->LHS->printAst(aout);
		aout<<")\n";

		aout<<"RHS (";
		this->RHS->printAst(aout);
		aout<<")";

		aout<<"\n";
	}

	//------------------------------------------------//
	void printTac(ostream& tout){
		if(!b_print){
			tout<<"**PROCEDURE: main\n**BEGIN: Three Address Code Statements\n";
			b_print = true;
		}
		any_tac = true;
		if(this->RHS->label==Num_E || this->RHS->label==Name_E || this->RHS->label==String_E){
			if(!silent){tout<<"\t";	}
			this->LHS->printTac(tout);
			if(!silent){tout<<" = ";}
			this->RHS->printTac(tout);
			if(!silent){tout<<"\n";	}
			this->index = -1;
		}
		else{
			this->RHS->printTac(tout);
			if(!silent){tout<<"\n\t"; }
			this->LHS->printTac(tout);
			if(!silent){
				if(!this->RHS->is_ter){
					tout<<" = temp"<<this->RHS->index<<"\n";
				}
				else{
					tout<<" = stemp"<<s-1<<"\n";
					//is_ter = false;
				}
			}
			this->index = this->RHS->index;
		}
	}
};
/////////////////////////////////
////////////////////////////////
////////////////////////////
///////////////////////
////////////////////
///////////
/////
///
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
		this->is_ter = false;

	}
	/*ostream& operator<<(ostream& tout){	
				tout<<t;	
			}*/
	void printAst(ostream& aout){
		aout<<"\nArith: ";

		switch(this->label){
			case Plus_E : {
				aout<<"Plus";
				break;
			}
			case Minus_E : {
				aout<<"Minus";
				break;
			}
			case Mult_E : {
				aout<<"Mult";
				break;
			}
			case Div_E : {
				aout<<"Div";
				break;
			}
			case Uminus_E : {
				aout<<"Uminus";
				break;
			}
		}

		switch(this->dtype){
			case INTe : {
				aout<<"<int>";
				break;
			}
			case FLOATe : {
				aout<<"<float>";
				break;
			}
			case BOOLe : {
				aout<<"<bool>";
				break;
			}
			case STRINGe : {
				aout<<"<string>";
				break;
			}
		}
		
		aout<<"\nL_Opd (";
		this->LHS->printAst(aout);
		aout<<")";

		if(this->RHS != NULL){ 
			aout<<"\nR_Opd (";
			this->RHS->printAst(aout);
			aout<<")";
		}
	}
	//------------------------------------------------//
	void printTac(ostream& tout){
		if(this->label!=Uminus_E){
			if(this->LHS->index==-1 && this->RHS->index==-1){
				if(!silent){
					tout<<"\ttemp"<<t<<" = ";
				}
				this->LHS->printTac(tout);
				if(!silent){
					switch(this->label){
						case Plus_E : {
							tout<<" + ";
							break;
						}
						case Minus_E : {
							tout<<" - ";
							break;
						}
						case Mult_E : {
							tout<<" * ";
							break;
						}
						case Div_E : {
							tout<<" / ";
							break;
						}
					}
				}	
				this->RHS->printTac(tout);
				this->index = t;
				t++;
			}
			else if(this->LHS->index!=-1 && this->RHS->index==-1){
				this->LHS->printTac(tout);
				if(!silent){	
					tout<<"\n\ttemp"<<t<<" = ";
					if(!this->LHS->is_ter){
						tout<<"temp"<<this->LHS->index;
					}
					else{
						tout<<"stemp"<<this->LHS->index;
						//is_ter = false;
					}
					//tout<<"temp"<<this->LHS->index;
					switch(this->label){
						case Plus_E : {
							tout<<" + ";
							break;
						}
						case Minus_E : {
							tout<<" - ";
							break;
						}
						case Mult_E : {
							tout<<" * ";
							break;
						}
						case Div_E : {
							tout<<" / ";
							break;
						}
					}
				}	
				this->RHS->printTac(tout);
				this->index = t;
				t++;
			}
			else if(this->LHS->index==-1 && this->RHS->index!=-1){
				this->RHS->printTac(tout);
				if(!silent){
					tout<<"\n\ttemp"<<t<<" = ";
				}
				this->LHS->printTac(tout);
				if(!silent){
					switch(this->label){
						case Plus_E : {
							tout<<" + ";
							break;
						}
						case Minus_E : {
							tout<<" - ";
							break;
						}
						case Mult_E : {
							tout<<" * ";
							break;
						}
						case Div_E : {
							tout<<" / ";
							break;
						}
					}	
					if(!this->RHS->is_ter){
						tout<<"temp"<<this->RHS->index;
					}
					else{
						tout<<"stemp"<<this->RHS->index;
						//is_ter = false;
					}
				}
				this->index = t;
				t++;
			}
			else{
				this->LHS->printTac(tout);	
				if(!silent){tout<<"\n"; }
				this->RHS->printTac(tout);
				if(!silent){
					tout<<"\n\ttemp"<<t;
					tout<<" = ";
					if(!this->LHS->is_ter){
						tout<<"temp"<<this->LHS->index;
					}
					else{
						tout<<"stemp"<<this->LHS->index;
						//is_ter = false;
					}
					switch(this->label){
						case Plus_E : {
							tout<<" + ";
							break;
						}
						case Minus_E : {
							tout<<" - ";
							break;
						}
						case Mult_E : {
							tout<<" * ";
							break;
						}
						case Div_E : {
							tout<<" / ";
							break;
						}
					}	
					if(!this->RHS->is_ter){
						tout<<"temp"<<this->RHS->index;
					}
					else{
						tout<<"stemp"<<this->RHS->index;
						//is_ter = false;
					}
				}
				this->index = t;
				t++;
			}
		}	
		else if(this->label==Uminus_E){
			if(this->LHS->index==-1){
				if(!silent){
					tout<<"\ttemp"<<t<<" = - ";
				}
				this->LHS->printTac(tout);
				this->index = t;
				t++;
			}
			else{
				this->LHS->printTac(tout);
				if(!silent){
					tout<<"\n\t";
					tout<<"temp"<<t<<" = - ";
					if(!this->LHS->is_ter){
						tout<<"temp"<<this->LHS->index;
					}
					else{
						tout<<"stemp"<<this->LHS->index;
						//is_ter = false;
					}
					//tout<<"temp"<<this->LHS->index;
				}
				this->index = t;
				t++;
			}
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
		this->is_ter = false;
	}
	
	void printAst(ostream& aout){		
		aout<<"\nCondition: ";
		switch(this->label){
			case GT_E : {
				aout<<"GT<bool>";
				break;
			}
			case GTE_E : {
				aout<<"GE<bool>";
				break;
			}
			case LT_E : {
				aout<<"LT<bool>";
				break;
			}
			case LTE_E : {
				aout<<"LE<bool>";
				break;
			}
			case Or_E : {
				aout<<"OR<bool>";
				break;
			}
			case And_E : {
				aout<<"AND<bool>";
				break;
			}
			case EQ_E : {
				aout<<"EQ<bool>";
				break;
			}
			case NEQ_E : {
				aout<<"NE<bool>";
				break;
			}
			case Not_E : {
				aout<<"NOT<bool>";
				break;
			}
		}

		aout<<"\nL_Opd (";
		this->LHS->printAst(aout);
		aout<<")";

		if(this->RHS != NULL){ 
			aout<<"\nR_Opd (";
			this->RHS->printAst(aout);
			aout<<")";
		}
	}
	//------------------------------------------------//
	void printTac(ostream& tout){
		if(this->label!=Not_E){
			if(this->LHS->index==-1 && this->RHS->index==-1){
				if(!silent){
					tout<<"\ttemp"<<t<<" = ";
				}
				this->LHS->printTac(tout);
				if(!silent){
					switch(this->label){
						case GT_E : {
							tout<<" > ";
							break;
						}
						case GTE_E : {
							tout<<" >= ";
							break;
						}
						case LT_E : {
							tout<<" < ";
							break;
						}
						case LTE_E : {
							tout<<" <= ";
							break;
						}
						case Or_E : {
							tout<<" || ";
							break;
						}
						case And_E : {
							tout<<" && ";
							break;
						}
						case EQ_E : {
							tout<<" == ";
							break;
						}
						case NEQ_E : {
							tout<<" != ";
							break;
						}	
					}
				}	
				this->RHS->printTac(tout);
				this->index = t;
				t++;
			}
			else if(this->LHS->index!=-1 && this->RHS->index==-1){
				this->LHS->printTac(tout);
				if(!silent){
					tout<<"\n\ttemp"<<t<<" = ";
					if(!this->LHS->is_ter){
						tout<<"temp"<<this->LHS->index;
					}
					else{
						tout<<"stemp"<<this->LHS->index;
						//is_ter = false;
					}
					switch(this->label){
						case GT_E : {
							tout<<" > ";
							break;
						}
						case GTE_E : {
							tout<<" >= ";
							break;
						}
						case LT_E : {
							tout<<" < ";
							break;
						}
						case LTE_E : {
							tout<<" <= ";
							break;
						}
						case Or_E : {
							tout<<" || ";
							break;
						}
						case And_E : {
							tout<<" && ";
							break;
						}
						case EQ_E : {
							tout<<" == ";
							break;
						}
						case NEQ_E : {
							tout<<" != ";
							break;
						}
					}
				}
				this->RHS->printTac(tout);
				this->index = t;
				t++;
			}
			else if(this->LHS->index==-1 && this->RHS->index!=-1){
				this->RHS->printTac(tout);
				if(!silent){
					tout<<"\n\ttemp"<<t<<" = ";
				}
				this->LHS->printTac(tout);
				if(!silent){
					switch(this->label){
						case GT_E : {
							tout<<" > ";
							break;
						}
						case GTE_E : {
							tout<<" >= ";
							break;
						}
						case LT_E : {
							tout<<" < ";
							break;
						}
						case LTE_E : {
							tout<<" <= ";
							break;
						}
						case Or_E : {
							tout<<" || ";
							break;
						}
						case And_E : {
							tout<<" && ";
							break;
						}
						case EQ_E : {
							tout<<" == ";
							break;
						}
						case NEQ_E : {
							tout<<" != ";
							break;
						}
					}	
					if(!this->RHS->is_ter){
						tout<<"temp"<<this->RHS->index;
					}
					else{
						tout<<"stemp"<<this->RHS->index;
						//is_ter = false;
					}
				}
				this->index = t;
				t++;
			}
			else{
				this->LHS->printTac(tout);	
				if(!silent){tout<<"\n"; }
				this->RHS->printTac(tout);
				if(!silent){
					tout<<"\n\ttemp"<<t;
					tout<<" = ";
					if(!this->LHS->is_ter){
						tout<<"temp"<<this->LHS->index;
					}
					else{
						tout<<"stemp"<<this->LHS->index;
						//is_ter = false;
					}
					switch(this->label){
						case GT_E : {
							tout<<" > ";
							break;
						}
						case GTE_E : {
							tout<<" >= ";
							break;
						}
						case LT_E : {
							tout<<" < ";
							break;
						}
						case LTE_E : {
							tout<<" <= ";
							break;
						}
						case Or_E : {
							tout<<" || ";
							break;
						}
						case And_E : {
							tout<<" && ";
							break;
						}
						case EQ_E : {
							tout<<" == ";
							break;
						}
						case NEQ_E : {
							tout<<" != ";
							break;
						}
					}	
					if(!this->RHS->is_ter){
						tout<<"temp"<<this->RHS->index;
					}
					else{
						tout<<"stemp"<<this->RHS->index;
						//is_ter = false;
					}
				}
				this->index = t;
				t++;
			}
		}	
		else if(this->label==Not_E){
			if(this->LHS->index==-1){
				if(!silent){
					tout<<"\ttemp"<<t;
					tout<<" = ! ";
				}
				this->LHS->printTac(tout);
				this->index = t;
				t++;
			}
			else{
				this->LHS->printTac(tout);
				if(!silent){
					tout<<"\n\ttemp"<<t;
					tout<<" = ! ";
					if(!this->LHS->is_ter){
						tout<<"temp"<<this->LHS->index;
					}
					else{
						tout<<"stemp"<<this->LHS->index;
						//is_ter = false;
					}
				}
				this->index = t;
				t++;
			}
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
		this->is_ter = true;
	}
	void printAst(ostream& aout){		
		this->Cond_part->printAst(aout);

		aout<<"\nTrue_Part (";
		this->True_Part->printAst(aout);
		aout<<")\n";

		aout<<"False_Part (";
		this->False_Part->printAst(aout);
		aout<<")";
	}
	//------------------------------------------------//
	void printTac(ostream& tout){

		if(!this->True_Part->is_ter && !this->False_Part->is_ter){
			if(!silent_ter && !silent_fal){	
				this->Cond_part->printTac(tout);
				this->Cond_part->index = t-1;
			}
			t_h = t;
			silent = true;
			//if(!silent_ter){	
			this->True_Part->printTac(tout);
			this->False_Part->printTac(tout);	
			//}
			silent = false;
			if(!silent_ter){
				if(!silent_fal){
				this->Cond_part->not_t = t;
				tout<<"\n\ttemp"<<this->Cond_part->not_t<<" = "<<"! temp"<<this->Cond_part->index;
				tout<<"\n\tif(temp"<<this->Cond_part->not_t<<") goto Label"<<l<<"\n";
				this->False_Part->load = l;
				t = t_h;
				l=l+2;
				}
				if(silent_fal){
				this->True_Part->store = s;
				}
				if(!silent_fal){
				if(this->True_Part->index==-1){
					tout<<"\tstemp"<<this->True_Part->store<<" = ";			
					this->True_Part->printTac(tout);
				}
    			else{
					this->True_Part->printTac(tout);
					tout<<"\n\tstemp"<<this->True_Part->store<<" = temp"<<this->True_Part->index;
				}
				}
    			s++;
    			if(!silent_fal){
    			tout<<"\n\tgoto Label"<<this->False_Part->load+1;
    			tout<<"\nLabel"<<this->False_Part->load<<":\n";
    			if(silent_fal){
    			this->store = this->True_Part->store;
    			}
    			this->False_Part->store = this->True_Part->store;
    			if(this->False_Part->index==-1){
    				tout<<"\tstemp"<<this->False_Part->store<<" = ";
    				this->False_Part->printTac(tout);
    			}
    			else{
    				this->False_Part->printTac(tout);
    				tout<<"\n\tstemp"<<this->False_Part->store<<" = temp"<<this->False_Part->index;
    			}
    			this->load = False_Part->load+1;
    			tout<<"\nLabel"<<this->False_Part->load+1<<":\t";
    			}
    			
    		}
    		//tout<<t;
    		t++;
    		//s++;
    		//tout<<t;
		}

		else if(this->True_Part->is_ter && !this->False_Part->is_ter){
			this->Cond_part->printTac(tout);
			this->Cond_part->index = t-1;
			t_h = t;
			//silent_fal = true;
			silent_ter = true;
			this->True_Part->printTac(tout);
			silent_ter = false;
			//silent_fal = false;

			silent = true;
			this->False_Part->printTac(tout);
			silent = false;
			//tout<<this->False_Part->index;
			tout<<"\n\ttemp"<<t+1<<" = "<<"! temp"<<this->Cond_part->index;
			tout<<"\n\tif(temp"<<t+1<<") goto Label"<<l<<"\n";
			t = t_h;
			this->False_Part->load = l;
			l=l+2;
			this->True_Part->printTac(tout);
			tout<<"\n\tstemp"<<s<<" = "<<"stemp"<<this->True_Part->store<<"\n";
			tout<<"\tgoto Label"<<this->False_Part->load+1;
			tout<<"\nLabel"<<this->False_Part->load<<":\n";
			this->False_Part->store = s;
			s++;
			if(this->False_Part->index==-1){
    			tout<<"\tstemp"<<this->False_Part->store<<" = ";
    			this->False_Part->printTac(tout);
    		}
    		else{
    			this->False_Part->printTac(tout);
    			tout<<"\n\tstemp"<<this->False_Part->store<<" = temp"<<this->False_Part->index;
    		}
			tout<<"\nLabel"<<this->False_Part->load+1<<":";
			this->store = s;
			t++;

		}
		else if(!this->True_Part->is_ter && this->False_Part->is_ter){
			this->Cond_part->printTac(tout);
			this->Cond_part->index = t-1;
			t_h = t;
			//silent_ter = true;
			//for_fal = true;
			silent_fal = true;
			this->False_Part->printTac(tout);
			silent_fal = false;
			//for_fal = false;
			//silent_ter = false;
			silent = true;
			this->True_Part->printTac(tout);
			silent = false;

			tout<<"\n\ttemp"<<t+1<<" = "<<"! temp"<<this->Cond_part->index;
			tout<<"\n\tif(temp"<<t+1<<") goto Label"<<l<<"\n";
			this->False_Part->load = l;
			l_h = l;
			l=l+2;
			t = t_h;
			this->True_Part->store = s;
			if(this->True_Part->index==-1){
    			tout<<"\tstemp"<<this->True_Part->store<<" = ";
    			this->True_Part->printTac(tout);
    		}
    		else{
    			this->True_Part->printTac(tout);
    			tout<<"\n\tstemp"<<this->True_Part->store<<" = temp"<<this->True_Part->index;
    		}
			tout<<"\n\tgoto Label"<<this->False_Part->load+1;
			tout<<"\nLabel"<<this->False_Part->load<<":\n";
			this->False_Part->printTac(tout);
			this->False_Part->load = l_h;
			tout<<"\n\tstemp"<<this->True_Part->store<<" = "<<"stemp"<<this->store;
			tout<<"\nLabel"<<this->False_Part->load+1<<":";
			this->store = this->True_Part->store;
			t++;
			//s++;
		}
		//cout<<t;
		/*tout<<"\n\ttemp"<<t<<" = "<<"! temp"<<this->Cond_part->index;
		tout<<"\n\tif(temp"<<t<<") goto Label"<<l;
		tout<<"\n";
		silent = false;
		t = t_h;

		if(this->True_Part->index==-1){
			tout<<"\tstemp"<<s<<" = ";			
			this->True_Part->printTac(tout);
		}
    	else{
			this->True_Part->printTac(tout);
			tout<<"\n\tstemp"<<s<<" = "<<"temp"<<t-1;
    	}

		tout<<"\n\tgoto Label"<<l+1;
		tout<<"\nLabel"<<l<<":\n";

		if(this->False_Part->index==-1){
			tout<<"\tstemp"<<s<<" = ";
    	this->False_Part->printTac(tout);
		}
		else{
			this->False_Part->printTac(tout);
			tout<<"\n\tstemp"<<s<<" = "<<"temp"<<t-1;
		}
		tout<<"\nLabel"<<l+1<<":\t";
		l=l+2;
		s=s+1;
		t=t+1;*/
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

	void printTree(ostream& aout){

	aout<<"Program:\n**PROCEDURE: main\n\tReturn Type: <void>\n\t\
Formal Parameters:\n";

	if(is_def){  ///////////////////////////////////////////////////////
		for(int i=0; i<param_dt2.size(); i++){
			aout<< *param_var2[i] << "_  Type:";

			switch(param_dt2[i]){
				case INTe : {
					aout<<"<int>\n";
					break;
				}
				case FLOATe : {
					aout<<"<float>\n";
					break;
				}
				case BOOLe : {
					aout<<"<bool>\n";
					break;
				}
				case STRINGe : {
					aout<<"<string>\n";
					break;
				}
			}
		}
	}
	else{  ////////////////////////////////////////////////////////////
		for(int i=0; i<param_dt1.size(); i++){
			aout<< *param_var1[i] << "_  Type:";

			switch(param_dt1[i]){
				case INTe : {
					aout<<"<int>\n";
					break;
				}
				case FLOATe : {
					aout<<"<float>\n";
					break;
				}
				case BOOLe : {
					aout<<"<bool>\n";
					break;
				}
				case STRINGe : {
					aout<<"<string>\n";
					break;
				}
			}
		}
	}

	/////////////////////////////////////////////////
	aout<<"**BEGIN: Abstract Syntax Tree\n";

		for(auto x:this->Ast_List){
			x->printAst(aout);
		}

		aout<<"**END: Abstract Syntax Tree\n";
	}
	/////--------------------------------------/////
	void printTacStmt(ostream& tout){
		for(auto x:this->Ast_List){
			x->printTac(tout);
		}
		if(any_tac){
			tout<<"**END: Three Address Code Statements\n";
		}
	}

};
/////////////////////////////////////////////////
