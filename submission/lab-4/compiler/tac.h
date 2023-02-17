#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;


// // #####################for TAC##############################

class Name_Tac : public Leaf_Tac{
public:
	string data;
	Name_Tac(string data){
		this->data = data;
	}

	void printTac(ostream& tout){
		tout<<this->data;
	}
	string fn_data(){
		return this->data;
	} 
};

class Int_Tac : public Leaf_Tac{
public:
	int data;
	Int_Tac(int data){
		this->data = data;
	}
	void printTac(ostream& tout){
		tout<<this->data;
	}
};

class Float_Tac : public Leaf_Tac{
public:
	float data;
	Float_Tac(float data){
		this->data = data;
	}	

	void printTac(ostream& tout){
		tout<<fixed<<setprecision(2)<<this->data;
	}
};

class Str_Tac : public Leaf_Tac{
public:
	string data;
	Str_Tac(string data){
		this->data = data;
	}
	void printTac(ostream& tout){
		tout<<this->data;
	}
};

//  main child node of TAC node 
class Move_Tac : public Tac_Node{
public:
	Leaf_Tac* LHS;
	Leaf_Tac* RHS;

	Move_Tac(Leaf_Tac* LHS, Leaf_Tac* RHS){
		this->LHS = LHS;
		this->RHS = RHS;
	}
	void printTac(ostream& tout){
		tout<<"\t";
		this->LHS->printTac(tout);
		tout<< " = ";
		this->RHS->printTac(tout);
		tout<< "\n";
	}
};

class Compute_Tac : public Tac_Node{
public:
	Name_Tac* LHS;
	NodeText Op;
	Leaf_Tac* L_Opd;
	Leaf_Tac* R_Opd;

	Compute_Tac(Name_Tac* LHS,NodeText Op,Leaf_Tac* L_Opd,Leaf_Tac* R_Opd){
		this->LHS = LHS;
		this->Op = Op;
		this->L_Opd = L_Opd;
		this->R_Opd = R_Opd;
	}

	void printTac(ostream& tout){
		tout<<"\t";
				
		this->LHS->printTac(tout);
		tout<<" = ";
		if(this->R_Opd == NULL){
			printEnumOp(tout, this->Op);
			tout<<" ";
			this->L_Opd->printTac(tout);
		}
		else{
			this->L_Opd->printTac(tout);
			tout<<" ";
			printEnumOp(tout, this->Op);
			tout<<" ";
			this->R_Opd->printTac(tout);
		}
		tout<<"\n";
	}
};

class Io_Tac : public Tac_Node{
public:
	NodeText type;       //print or write
	Leaf_Tac* data;

	Io_Tac(NodeText type, Leaf_Tac* data){
		this->type = type;
		this->data = data;
	}
	void printTac(ostream& tout){
		tout<<"\t";

		if(this->type == READ_E){
			tout<<"read  ";
		}
		else{
			tout<<"write  ";
		}
		this->data->printTac(tout);
		tout<<"\n";
	}
};

class Control_flow_Tac : public Tac_Node{
public:
	string LHS;     //if(temp0)
	string RHS;     // goto label0

	Control_flow_Tac(string LHS, string RHS){
		this->LHS = LHS;
		this->RHS = RHS;
	}
	void printTac(ostream& tout){
		tout<<"\t";

		if((this->LHS).length() ==0){
			// goto Label1
			tout<<"goto " + this->RHS << "\n";
		}
		else{
			// if(temp5) goto Label0
			tout<<"if(" + this->LHS + ") goto " + this->RHS << "\n";
		}
	}
};

class Label_Tac : public Tac_Node{
public:
	string data;     // label0:

	Label_Tac(string data){
		this->data = data;
	}
	void printTac(ostream& tout){
		// Label0:
		tout<<this->data<<":\n";
	}
};

// #####################for TAC END##########################
