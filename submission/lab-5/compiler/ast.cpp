#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "ast.h"
using namespace std;

// external variable ###########################################
extern void semantic_error();


// member function & constructor ################################
void Ast_List_class::printAst(ostream& aout){
	for(auto x:this->Ast_List){
		x->printAst(aout);
	}
}

Name_Ast::Name_Ast(DataType dtype, NodeText label, SymTablePtr data){
	this->dtype = dtype;
	this->label = label;
	this->data = data;
}

void Name_Ast::printAst(ostream& aout){
	aout<<"Name : "<<this->data->first<<"_";
	printEnumDT(aout, this->data->second);
}

Int_Ast::Int_Ast(DataType dtype, NodeText label, int data){
	this->dtype = dtype;
	this->label = label;
	this->data = data;
}

void Int_Ast::printAst(ostream& aout){
	aout<<"Num : "<<this->data<<"<int>";
}

Float_Ast::Float_Ast(DataType dtype, NodeText label, double data){
	this->dtype = dtype;
	this->label = label;
	this->data = data;
}

void Float_Ast::printAst(ostream& aout){
	aout<<"Num : "<<fixed<<setprecision(2)<<this->data<<"<float>";
}

Str_Ast::Str_Ast(DataType dtype, NodeText label, string data){
	this->dtype = dtype;
	this->label = label;
	this->data = data;
}

void Str_Ast::printAst(ostream& aout){
	aout<<"String : "<<this->data<<"<string>";
}

Io_Ast::Io_Ast(DataType dtype, NodeText label, Exp_Ast* data){
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

void Io_Ast::printAst(ostream& aout){
	aout<<"\n";
	printEnumIO(aout,this->label);
	data->printAst(aout);
}

Asgn_Ast::Asgn_Ast(DataType dtype, NodeText label, Name_Ast* LHS, Exp_Ast* RHS){
	if (LHS->dtype != RHS->dtype){ 
		semantic_error();
	}
	this->dtype = dtype;
	this->label = label;
	this->LHS = LHS;
	this->RHS = RHS;
}

void Asgn_Ast::printAst(ostream& aout){
	aout<<"\nAsgn:\n";
	
	aout<<"LHS (";
	this->LHS->printAst(aout);
	aout<<")\n";

	aout<<"RHS (";
	this->RHS->printAst(aout);
	aout<<")";
}

Arith_Ast::Arith_Ast(DataType dtype, NodeText label, Exp_Ast* LHS, Exp_Ast* RHS){
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

void Arith_Ast::printAst(ostream& aout){
	aout<<"\nArith: ";
	printEnumArith(aout, this->label);

	printEnumDT(aout, this->dtype);
	
	aout<<"\nL_Opd (";
	this->LHS->printAst(aout);
	aout<<")";

	if(this->RHS != NULL){ 
		aout<<"\nR_Opd (";
		this->RHS->printAst(aout);
		aout<<")";
	}
}

Relat_Ast::Relat_Ast(DataType dtype, NodeText label, Exp_Ast* LHS, Exp_Ast* RHS){
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

void Relat_Ast::printAst(ostream& aout){		
	aout<<"\nCondition: ";
	printEnumRelat(aout, this->label);

	aout<<"\nL_Opd (";
	this->LHS->printAst(aout);
	aout<<")";

	if(this->RHS != NULL){ 
		aout<<"\nR_Opd (";
		this->RHS->printAst(aout);
		aout<<")";
	}
}

Qmark_Ast::Qmark_Ast(DataType dtype, NodeText label, 
	Exp_Ast* cp, Exp_Ast* tp, Exp_Ast* fp, int si){

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
	this->stemp_index = si;
}

void Qmark_Ast::printAst(ostream& aout){		
	this->Cond_part->printAst(aout);

	aout<<"\nTrue_Part (";
	this->True_Part->printAst(aout);
	aout<<")\n";

	aout<<"False_Part (";
	this->False_Part->printAst(aout);
	aout<<")";
}

While_Ast::While_Ast(DataType dtype, NodeText label,
		  Exp_Ast* cond_part,Ast_List_class* body_part){

	if (cond_part->dtype != BOOLe){ 
		semantic_error();
	}
	this->dtype = dtype;
	this->label = label;
	this->cond_part = cond_part;
	this->body_part = body_part;
}

void While_Ast::printAst(ostream& aout){
	if(this->label == DO_WHILE_E){
		aout<<"\nDo:";

		aout<<"\nBody (";
		this->body_part->printAst(aout);
		aout<<")";

		aout<<"\nWhile Condition (";
		this->cond_part->printAst(aout);
		aout<<")";
	}
	else{
		aout<<"\nWhile:";
		
		aout<<"\nCondition (";
		this->cond_part->printAst(aout);
		aout<<")";

		aout<<"\nBody (";
		this->body_part->printAst(aout);
		aout<<")";
	}
}

If_Else_Ast::If_Else_Ast(DataType dtype, NodeText label,
		  Exp_Ast* cond_part,
		  Ast_List_class* true_Part,Ast_List_class* false_Part){
	
	if (cond_part->dtype != BOOLe){ 
		semantic_error();
	}
	this->dtype = dtype;
	this->label = label;
	this->cond_part = cond_part;
	this->true_Part = true_Part;
	this->false_Part = false_Part;
}

void If_Else_Ast::printAst(ostream& aout){
	aout<<"\nIf:";

	aout<<"\nCondition (";
	this->cond_part->printAst(aout);
	aout<<")";

	aout<<"\nThen (";
	this->true_Part->printAst(aout);
	aout<<")";

	if(this->false_Part != NULL){ 
		aout<<"\nElse (";
		this->false_Part->printAst(aout);
		aout<<")";
	}
}
