#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "ast.h"
#include "tac.h"
// #include "rtl.h"
using namespace std;

extern vector<int> temp_toReg;
Str_Map_class* str_list = new Str_Map_class();

// Helper Functions ###########################################
void printEnumOpRtl(ostream& fout,NodeText label, bool is_float){

	switch(label){
		// arith Operator
		case Plus_E : {
			fout<<"add";
			break;
		}
		case Minus_E : {
			fout<<"sub";
			break;
		}
		case Mult_E : {
			fout<<"mul";
			break;
		}
		case Div_E : {
			fout<<"div";
			break;
		}
		case Uminus_E : {
			fout<<"uminus";
			break;
		}

		// relational Operator
		case GT_E : {
			if(is_float){ 
				fout<<"sle";    // sgt not exist in float
			}
			else{
				fout<<"sgt";
			}
			break;
		}
		case GTE_E : {   
			if(is_float){ 
				fout<<"slt";    // sge not exist in float
			}
			else{
				fout<<"sge";
			}
			break;
		}
		case LT_E : {
			fout<<"slt";
			break;
		}
		case LTE_E : {
			fout<<"sle";
			break;
		}
		case Or_E : {
			fout<<"or";
			break;
		}
		case And_E : {
			fout<<"and";
			break;
		}
		case EQ_E : {
			fout<<"seq";
			break;
		}
		case NEQ_E : {
			if(is_float){ 
				fout<<"seq";    // sne not exist in float
			}
			else{
				fout<<"sne";
			}
			break;
		}
		case Not_E : {
			fout<<"not";
			break;
		}
	}
}


// member function ################################
void Reg_alloc_class::makeReg(){
	reg_isused_vec = vector<bool>(36, false);
}

void Reg_alloc_class::freeReg(int regindex){
	if(regindex >=0 && regindex <=35){
		this->reg_isused_vec[regindex]=false;
	}
}

int Reg_alloc_class::getRegister(){
	int i = 0;     // 0-18 available
	while(i<=18){ 
		if(this->reg_isused_vec[i] == false){   // this reg is free
			this->reg_isused_vec[i] = true;    //now use this reg
			return i;
		}
		i++; 
	}
	fprintf(stderr,"Register out of bound!\n");
	exit(1);
	return 0;
}

int Reg_alloc_class::getRegister_F(){
	int i = 20;        // 20-34 available in case of float
	while(i<=34){ 
		if(this->reg_isused_vec[i] == false){   // this reg is free
			this->reg_isused_vec[i] = true;    //now use this reg
			return i;
		}
		i++; 
	}
	fprintf(stderr,"Floating register out of bound!\n");
	exit(1);
	return 20;
}

void Reg_alloc_class::printReg(ostream& rout, int regindex){
	if(regindex < 0){
		cout<<"invalid reg : "<<regindex<<endl;
	}
	else if(regindex <= 19){ 
		//  normal register
		if(regindex == 0) { rout<<"v0"; }                   // v0
		else if(regindex <= 10) { rout<<"t"<<regindex-1; }  // t0-t9
		else if(regindex <= 18) { rout<<"s"<<regindex-11; } // s0-s7
		else if(regindex == 19) { rout<<"a0"; }             // a0
		else{}
	}
	else if(regindex <= 35){ 
		//  floating register
		if(regindex <= 34) { rout<<"f"<<(regindex-19)*2; }  // f2-f30  (f12 also)
		else if(regindex == 35) { rout<<"f0"; }             // f0
		else{}
	}
	else{} 
}


// member function for making RTL list using insertRtl###################
int Name_Tac::insertRtl(vector<Rtl_Node*> &Rtl_list, int regindex){
	if((this->data)[0]=='t' && (this->data)[(this->data).size()-1] != '_'){
		//in case of tempx , take the reg index from temp_toReg vector
		return temp_toReg[ stoi((this->data).substr(4)) ];
	}
	int comment = -1;
	if (regindex < 0){    //get new reg if not provided
		 if(this->dtype == FLOATe){
			 regindex = Reg_alloc_obj->getRegister_F(); 
		 }
		 else{
			 regindex = Reg_alloc_obj->getRegister(); 
		 }
	 }
	 else if(regindex >108) { regindex -=108; comment = 108;}  //jugad for commenting

	Name_Rtl* newRHS = new  Name_Rtl(this->data);
	Load_Rtl* newnode = new Load_Rtl(regindex, newRHS, false, comment);
	Rtl_list.push_back(newnode);
	return regindex;
}
int Int_Tac::insertRtl(vector<Rtl_Node*> &Rtl_list, int regindex){
	int comment = 101;
	if (regindex < 0){ regindex = Reg_alloc_obj->getRegister(); }
	else if(regindex >108) { regindex -=108; comment = 108;}  //jugad for commenting

	Int_Rtl* newRHS = new Int_Rtl(this->data);
	Load_Rtl* newnode = new Load_Rtl(regindex, newRHS, true,comment);
	Rtl_list.push_back(newnode);
	return regindex;
}
int Float_Tac::insertRtl(vector<Rtl_Node*> &Rtl_list, int regindex){
	int comment = 102;
	if (regindex < 0){ regindex = Reg_alloc_obj->getRegister_F(); }
	else if(regindex >108) { regindex -=108; comment = 108;}  //jugad for commenting

	Float_Rtl* newRHS = new Float_Rtl(this->data);
	Load_Rtl* newnode = new Load_Rtl(regindex, newRHS, true,comment);
	Rtl_list.push_back(newnode);
	return regindex;
}
int Str_Tac::insertRtl(vector<Rtl_Node*> &Rtl_list, int regindex){
	if (regindex < 0){ regindex = Reg_alloc_obj->getRegister(); }

	Load_str_Rtl* newnode = new Load_str_Rtl(regindex, str_list->insertStr(this->data));
	Rtl_list.push_back(newnode);
	return regindex;
}
int Move_Tac::insertRtl(vector<Rtl_Node*> &Rtl_list, int regindex){
	Name_Rtl*  newLHS   = new Name_Rtl(this->LHS->data);
	int        newRHS   = this->RHS->insertRtl(Rtl_list,-1);
	Store_Rtl* newnode = new Store_Rtl(newLHS, newRHS,-1);

	Reg_alloc_obj->freeReg(newRHS); //calculation done, now free reg
	Rtl_list.push_back(newnode);
	return -1;
}
int Compute_Tac::insertRtl(vector<Rtl_Node*> &Rtl_list, int regindex){
	int regnum1, regnum2, regnum3, regnum4 = -1, regnum5;

	bool is_special = ((this->L_Opd->dtype == FLOATe) && 
		 (this->Op == GT_E || this->Op == GTE_E || this->Op == LT_E || 
	      this->Op == LTE_E || this->Op == EQ_E || this->Op == NEQ_E)); 

	regnum1 = this->L_Opd->insertRtl(Rtl_list, -1);
	if(is_special){     // special case for float
		regnum2 = -1;   //there is no result OPrand in case of float
	}
	else{
		if(this->L_Opd->dtype == FLOATe){
			regnum2 = Reg_alloc_obj->getRegister_F();
		}
		else{
			regnum2 = Reg_alloc_obj->getRegister();
		}
		//  convert tempx to rx 
		// first get int x from string tempx
		temp_toReg[ stoi((this->LHS->data).substr(4)) ] = regnum2;

	}
	if(this->R_Opd == NULL){
		regnum3 = -1;
	}
	else{
		regnum3 = this->R_Opd->insertRtl(Rtl_list, -1);
	}


	Compute_Rtl* newnode = new Compute_Rtl(regnum2, this->Op,regnum1, regnum3);
	Rtl_list.push_back(newnode);

	if(is_special){
	    // special case for float

		Int_Rtl* newRHS = new Int_Rtl(1);
		regnum5 = Reg_alloc_obj->getRegister();
		Load_Rtl* newnode1 = new Load_Rtl(regnum5, newRHS, true,-1);   

		Rtl_list.push_back(newnode1);
		regnum4 = Reg_alloc_obj->getRegister();
		temp_toReg[ stoi((this->LHS->data).substr(4)) ] = regnum4;
		Move_Rtl* newnode2 = new Move_Rtl(regnum4, 0);
		Rtl_list.push_back(newnode2);
		if(this->Op == EQ_E || this->Op == LTE_E || this->Op == LT_E){
			Movt_Rtl* newnode3 = new Movt_Rtl(regnum4, regnum5, 0);
			Rtl_list.push_back(newnode3);
		}
		else{
			Movf_Rtl* newnode3 = new Movf_Rtl(regnum4, regnum5, 0);
			Rtl_list.push_back(newnode3);
		}

		Reg_alloc_obj->freeReg(regnum5); //calculation done, now free reg
	}


	Reg_alloc_obj->freeReg(regnum1); //calculation done, now free reg
	Reg_alloc_obj->freeReg(regnum3); //calculation done, now free reg

	return -1;
}
int Io_Tac::insertRtl(vector<Rtl_Node*> &Rtl_list, int regindex){
	if(this->type == READ_E){
		Int_Rtl* newRHS = new Int_Rtl(5);
		Load_Rtl* newnode1 = new Load_Rtl(0, newRHS, true,-1); //v0 just for syscall
		Read_Rtl* newnode2 = new Read_Rtl();
		Name_Rtl* newLHS = new Name_Rtl(this->data->fn_data());
		Store_Rtl* newnode3 = new Store_Rtl(newLHS,0,101);


		if(this->data->dtype == INTe){
			newRHS->data  = 5;     // 5 for read int
			newnode3->RHS = 0;     // v0  for input value
			newnode1->comment = 106;   
		}
		else if(this->data->dtype == FLOATe){
			newRHS->data  = 7;     // 7 for read float
			newnode3->RHS = 35;    // f0  for input value
			newnode1->comment = 107;   
		}
		else{ return -1;}   // only int & float are allowed in read
		Rtl_list.push_back(newnode1);
		Rtl_list.push_back(newnode2);
		Rtl_list.push_back(newnode3);
	}
	else{

		Int_Rtl* newRHS = new Int_Rtl(1);
		int regnum = 19;    // 19 for a0, 25 for f12
		int comment, comment2;

		if(this->data->dtype == INTe){
			newRHS->data  = 1;     // 5 for print int
			comment = 103;
			comment2 = 108;
		}
		else if(this->data->dtype == FLOATe){
			newRHS->data  = 3;     // 3 for print float
			regnum = 25;
			comment = 104;
			comment2 = 108;
		}
		else if(this->data->dtype == STRINGe){
			newRHS->data  = 4;     // 4 for print string
			comment = 105;
			comment2 = 0;
		}
		else{ return -1;}   // only int , float , string are allowed in print

		Load_Rtl* newnode1 = new Load_Rtl(0, newRHS, true,comment);  // v0 used here , for syscall
		Rtl_list.push_back(newnode1);

		this->data->insertRtl(Rtl_list, regnum+comment2);   // +0 or +108
		Write_Rtl* newnode2 = new Write_Rtl();
		Rtl_list.push_back(newnode2);
	}
	return -1;
}
int Control_flow_Tac::insertRtl(vector<Rtl_Node*> &Rtl_list, int regindex){
	if((this->LHS).length() ==0){
		// goto Label1
		Goto_Rtl* newnode = new Goto_Rtl(this->RHS);
		Rtl_list.push_back(newnode);
	}
	else{
		// if(temp5) goto Label0
		// bgtz: t0 , Label0
		Name_Tac* newnode1 = new Name_Tac(INTe, this->LHS);
		int regnum = newnode1->insertRtl(Rtl_list, -1);
		Bgtz_Rtl* newnode2 = new Bgtz_Rtl(regnum, this->RHS);
		
		Reg_alloc_obj->freeReg(regnum); //calculation done, now free reg
		Rtl_list.push_back(newnode2);
	}
	return -1;
}
int Label_Tac::insertRtl(vector<Rtl_Node*> &Rtl_list, int regindex){
	Label_Rtl* newnode = new Label_Rtl(this->data);
	Rtl_list.push_back(newnode);
	return -1;
}

	// NodeText type;       //print or write
	// Leaf_Tac* data;
