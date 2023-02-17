#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <iomanip>
#include "sclp.h"
using namespace std;

typedef unordered_map<string, int> StrMap;
typedef StrMap::iterator StrMapItr;
extern int str_index;
class Reg_alloc_class;
extern Reg_alloc_class* Reg_alloc_obj;

void printEnumOpRtl(ostream& fout,NodeText label, bool is_float);
// // #####################Helper class###########################
class Str_Map_class{
public:
	StrMap umap;

	StrMapItr insertStr(string s){
		StrMapItr theptr = (this->umap).find(s);
		if(theptr==(this->umap).end()){
			// key not found
			str_index++;
			theptr = (this->umap).insert({s,str_index}).first;
			return theptr;
		}
		else{
			return theptr;
		}
	}
};

class Reg_alloc_class{
public:
	vector<bool>  reg_isused_vec;

	Reg_alloc_class(){}
	void makeReg();
	int getRegister();
	int getRegister_F();       // for FLOAT
	void freeReg(int regindex);
	void printReg(ostream& rout, int regindex);
};

// // #####################for RTL##############################

//base class for RTL
class Rtl_Node{
public:
	virtual void printRtl(ostream& rout){}
};

// type of leaf node in rtl 
class Leaf_Rtl : public Rtl_Node{
public:
	int comment;
	void printRtl(ostream& rout){}
	virtual int fun_data(){}
};


class Name_Rtl : public Leaf_Rtl{
public:
	string data;
	Name_Rtl(string data){
		this->data = data;
	}

	void printRtl(ostream& rout){
		rout<<this->data;
	}
};

class Int_Rtl : public Leaf_Rtl{
public:
	int data;
	Int_Rtl(int data){
		this->data = data;
		this->comment = -1;
	}
	void printRtl(ostream& rout){
		if(this->comment == 101){
			rout<<"\t;; Loading integer number "<<this->data;
		}
		else{
			rout<<this->data;
		}
	}
	int fun_data(){ return this->data; }
};

class Float_Rtl : public Leaf_Rtl{
public:
	double data;
	Float_Rtl(double data){
		this->data = data;
		this->comment = -1;
	}	

	void printRtl(ostream& rout){
		if(this->comment == 102){
			rout<<"\t;; Loading float number ";
			rout<<fixed<<setprecision(6)<<this->data;
		}
		else{
			rout<<fixed<<setprecision(2)<<this->data;
		}
	}
};

//  main child node of RTL node 
class Compute_Rtl : public Rtl_Node{
public:
	int LHS;
	NodeText Op;
	int L_Opd;
	int R_Opd;

	Compute_Rtl(int LHS,NodeText Op,int L_Opd,int R_Opd){
		this->LHS = LHS;
		this->Op = Op;
		this->L_Opd = L_Opd;
		this->R_Opd = R_Opd;
	}

	void printRtl(ostream& rout){
		rout<<"\t";
		bool is_float = this->L_Opd >= 20;
		printEnumOpRtl(rout, this->Op, is_float);
		if(is_float){
			rout<<".d";
		}
		rout<<":\t";

		if(this->LHS >= 0){
			Reg_alloc_obj->printReg(rout, this->LHS);
			rout<<" <- ";
		}
		Reg_alloc_obj->printReg(rout, this->L_Opd);
		if(this->R_Opd >= 0){
			rout<<" , ";
			Reg_alloc_obj->printReg(rout, this->R_Opd);
		}

		if(is_float && (this->Op==GT_E || this->Op==GTE_E || this->Op==NEQ_E)){
			rout<<";; Negating the condition and using le for gt, lt for ge,\
and eq for neq, for float values\n\
;; because gt, ge, and neq operations are not supported by the coprocessor";
		}

		rout<<"\n";
	}
};

class Read_Rtl : public Rtl_Node{
public:
	void printRtl(ostream& rout){
		rout<<"\tread\t;; This is where syscall will be made\n";
	}
};

class Write_Rtl : public Rtl_Node{
public:
	void printRtl(ostream& rout){
		rout<<"\twrite\t;; This is where syscall will be made\n";
	}
};

class Bgtz_Rtl : public Rtl_Node{
public:
	int LHS;
	string RHS;     // label name

	Bgtz_Rtl(int LHS, string RHS){
		this->LHS = LHS;
		this->RHS = RHS;
	}
	void printRtl(ostream& rout){
		rout<<"\tbgtz:\t";
		Reg_alloc_obj->printReg(rout, this->LHS);
		rout<<" , "<<this->RHS<<"\n";
	}
};

class Goto_Rtl : public Rtl_Node{
public:
	string data;     // label name

	Goto_Rtl(string data){
		this->data = data;
	}
	void printRtl(ostream& rout){
		rout<<"\tgoto:\t"<<this->data<<"\n";
	}
};

class Label_Rtl : public Rtl_Node{
public:
	string data;     // label0:

	Label_Rtl(string data){
		this->data = data;
	}
	void printRtl(ostream& rout){
		// Label0:
		rout<<"\n"<<this->data<<":\n";
	}
};

class Move_Rtl : public Rtl_Node{
public:
	int LHS;
	int RHS;

	Move_Rtl(int LHS,int RHS){
		this->LHS = LHS;
		this->RHS = RHS;
	}
	void printRtl(ostream& rout){
		// move: t0 <- zero    
		rout<<"\tmove: ";
		Reg_alloc_obj->printReg(rout, this->LHS);
		rout<< " <- ";
		if(this->RHS == 0){
			rout<<"zero\n";
		}
		else{
			Reg_alloc_obj->printReg(rout, this->RHS);
			rout<<"\n";
		}
	}
};

class Movf_Rtl : public Rtl_Node{
public:
	int result;
	int LHS;
	int RHS;

	Movf_Rtl(int result, int LHS,int RHS){
		this->result = result;
		this->LHS = LHS;
		this->RHS = RHS;
	}
	void printRtl(ostream& rout){
		// movf: t0 <- v0 , 0    
		rout<<"\tmovf: ";
		Reg_alloc_obj->printReg(rout, this->result);
		rout<< " <- ";
		Reg_alloc_obj->printReg(rout, this->LHS);
		rout<< " , " << RHS << "\n";
	}
};

class Movt_Rtl : public Rtl_Node{
public:
	int result;
	int LHS;
	int RHS;

	Movt_Rtl(int result, int LHS,int RHS){
		this->result = result;
		this->LHS = LHS;
		this->RHS = RHS;
	}
	void printRtl(ostream& rout){
		// movt: t0 <- v0 , 0    
		rout<<"\tmovt: ";
		Reg_alloc_obj->printReg(rout, this->result);
		rout<< " <- ";
		Reg_alloc_obj->printReg(rout, this->LHS);
		rout<< " , " << RHS << "\n";
	}
};


class Load_str_Rtl : public Rtl_Node{
public:
	int LHS;
	StrMapItr umap_itr;

	Load_str_Rtl(int LHS,StrMapItr p){
		this->LHS = LHS;
		this->umap_itr = p;
	}

	void printRtl(ostream& rout){
		// load_addr:    	v0 <- _str_0 ;; String = "smtg here"
		rout<<"\tload_addr: ";
		Reg_alloc_obj->printReg(rout, this->LHS);
		rout<< " <- _str_"<< this->umap_itr->second;
		rout<<" ;; String = "<<this->umap_itr->first<<"\n";
	}
};


class Load_Rtl : public Rtl_Node{
public:
	int  LHS;
	Leaf_Rtl* RHS;
	bool is_number;     // load or iload
	int comment;

	Load_Rtl(int LHS, Leaf_Rtl* RHS, bool in, int comment){
		this->LHS = LHS;
		this->RHS = RHS;
		this->is_number = in;
		this->comment = comment;
	}

	void printRtl(ostream& rout){
		// iLoad.d:    fxx <- fnum
		// load:       rxx <- NAME
		rout<<"\t";
		if(this->is_number){
			rout<<"iLoad";
		}
		else{
			rout<<"load";
		}
		if(this->LHS >= 20){
			rout<<".d";
		}
		rout<<":\t";
		Reg_alloc_obj->printReg(rout, this->LHS);
		rout<<" <- ";
		this->RHS->printRtl(rout);

		if(this->comment == 101){
			this->RHS->comment = 101;
			this->RHS->printRtl(rout);  //just to print comment 
		}
		else if(this->comment == 102){
			this->RHS->comment = 102;
			this->RHS->printRtl(rout);  //just to print comment 
		}
		else if(this->comment >= 103 && this->comment <= 107){
			rout<<";; Loading "<<this->RHS->fun_data()<<" in ";
			Reg_alloc_obj->printReg(rout, this->LHS);
			rout<<" to indicate syscall to ";

			if(this->comment == 103){
				rout<<"print integer value";
			}
			else if(this->comment == 104){
				rout<<"print double value";
			}
			else if(this->comment == 105){
				rout<<"print string value";
			}
			else if(this->comment == 106){
				rout<<"read integer value";
			}
			else if(this->comment == 107){
				rout<<"read double value";
			} else{}

		}
		else if(this->comment == 108){
			rout<<";; Moving the value to be printed into register ";
			Reg_alloc_obj->printReg(rout, this->LHS);
		}
		else if(this->comment == 109){
			rout<<";; Moving the read value from register ";
			Reg_alloc_obj->printReg(rout, this->LHS);
		}
		else{}
		rout<<"\n";
	}
};

class Store_Rtl : public Rtl_Node{
public:
	Name_Rtl* LHS;
	int  RHS;
	int comment;

	Store_Rtl(Name_Rtl* LHS, int RHS, int comment){
		this->LHS = LHS;
		this->RHS = RHS;
		this->comment = comment;
	}

	void printRtl(ostream& rout){
		// store.d:    NAME <- fxx
		rout<<"\tstore";
		if(this->RHS >= 20){
			rout<<".d";
		}
		rout<<":\t";
		this->LHS->printRtl(rout);
		rout<<" <- ";
		Reg_alloc_obj->printReg(rout, this->RHS);
		if(this->comment == 101){
			rout<<";; Moving the read value from register ";
			Reg_alloc_obj->printReg(rout, this->RHS);
		}
		rout<<"\n";
	}
};
// #####################for RTL END##########################
