#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "ast.h"
#include "tac.h"
using namespace std;

// external variable ###########################################
extern int temp_index;
extern int label_index;


// member function ################################
Leaf_Tac* Ast_List_class::insertTac(vector<Tac_Node*> &Tac_list){
	for(auto x:this->Ast_List){
		x->insertTac(Tac_list);
	}
}

Leaf_Tac* Name_Ast::insertTac(vector<Tac_Node*> &Tac_list){
	Name_Tac* newnode  = new Name_Tac(this->data->first + "_");
	return newnode;
}

Leaf_Tac* Int_Ast::insertTac(vector<Tac_Node*> &Tac_list){
	Int_Tac* newnode  =  new Int_Tac(this->data);
	return newnode;
}

Leaf_Tac* Float_Ast::insertTac(vector<Tac_Node*> &Tac_list){
	Float_Tac* newnode  =  new Float_Tac(this->data);
	return newnode;
}

Leaf_Tac* Str_Ast::insertTac(vector<Tac_Node*> &Tac_list){
	Str_Tac* newnode  =  new Str_Tac(this->data);
	return newnode;
}

Leaf_Tac* Io_Ast::insertTac(vector<Tac_Node*> &Tac_list){
	Io_Tac* newnode  =  new Io_Tac(this->label, this->data->insertTac(Tac_list));
	Tac_list.push_back(newnode);
	return NULL;
}

Leaf_Tac* Asgn_Ast::insertTac(vector<Tac_Node*> &Tac_list){
	Move_Tac* newnode  =  new Move_Tac(
		this->LHS->insertTac(Tac_list), 
		this->RHS->insertTac(Tac_list));

	Tac_list.push_back(newnode);
	return NULL;
}

Leaf_Tac* Arith_Ast::insertTac(vector<Tac_Node*> &Tac_list){
	Compute_Tac* newnode;
	if(RHS == NULL){ 
		newnode  =  new Compute_Tac(NULL , this->label, this->LHS->insertTac(Tac_list), NULL);
	}
	else{
		newnode  =  new Compute_Tac(NULL , this->label, this->LHS->insertTac(Tac_list), this->RHS->insertTac(Tac_list));
	}
	string new_temp = "temp"+to_string(temp_index);
	Name_Tac* newname = new  Name_Tac(new_temp);
	temp_index++;
	newnode->LHS = newname;
	Tac_list.push_back(newnode);
	return newname;
}

Leaf_Tac* Relat_Ast::insertTac(vector<Tac_Node*> &Tac_list){
	Compute_Tac* newnode;
	if(RHS == NULL){ 
		newnode  =  new Compute_Tac(NULL , this->label, this->LHS->insertTac(Tac_list), NULL);
	}
	else{
		newnode  =  new Compute_Tac(NULL , this->label, this->LHS->insertTac(Tac_list), this->RHS->insertTac(Tac_list));
	}
	
	string new_temp = "temp"+to_string(temp_index);
	Name_Tac* newname = new  Name_Tac(new_temp);
	temp_index++;
	newnode->LHS = newname;
	Tac_list.push_back(newnode);
	return newname;

}

Leaf_Tac* Qmark_Ast::insertTac(vector<Tac_Node*> &Tac_list){
	Leaf_Tac* L_Opd = this->Cond_part->insertTac(Tac_list);
	
	vector<Tac_Node*> Tac_list_T;
	vector<Tac_Node*> Tac_list_F;

	string new_label0 = "Label"+to_string(label_index);
	label_index++;
	string new_label1 = "Label"+to_string(label_index);
	label_index++;

	Leaf_Tac* true_result = this->True_Part->insertTac(Tac_list_T);
	Leaf_Tac* false_result = this->False_Part->insertTac(Tac_list_F);

	// add negation of cond
	string new_temp = "temp"+to_string(temp_index);
	temp_index++;
	Name_Tac* newname = new  Name_Tac(new_temp);
	Compute_Tac* newnode = new Compute_Tac(newname, Not_E,L_Opd, NULL);

	Tac_list.push_back(newnode);

	// add if(temp0) goto label0;
	Control_flow_Tac* newgoto = new Control_flow_Tac(new_temp, new_label0);
	
	Tac_list.push_back(newgoto);
	
	// add true part tac list
	Tac_list.insert(Tac_list.end(), Tac_list_T.begin(), Tac_list_T.end());

	//  add stemp0 = tempt
	string new_stemp = "stemp"+to_string(this->stemp_index);
	Name_Tac* new_sname = new  Name_Tac(new_stemp);

	Move_Tac* newmove = new Move_Tac(new_sname, true_result);

	Tac_list.push_back(newmove);

	// add goto Label1

	newgoto = new Control_flow_Tac("", new_label1);
	
	Tac_list.push_back(newgoto);


	// add Label0:
	Label_Tac* newlabel_node  = new Label_Tac(new_label0);
	Tac_list.push_back(newlabel_node);

	// add false part tac list
	Tac_list.insert(Tac_list.end(), Tac_list_F.begin(), Tac_list_F.end());

	//  add stemp0 = tempf
	newmove = new Move_Tac(new_sname, false_result);

	Tac_list.push_back(newmove);

	// add Label1:
	newlabel_node  = new Label_Tac(new_label1);
	Tac_list.push_back(newlabel_node);


	return new_sname;		
}

Leaf_Tac* While_Ast::insertTac(vector<Tac_Node*> &Tac_list){
	if(this->label == DO_WHILE_E){
		vector<Tac_Node*> Tac_list_B;
		vector<Tac_Node*> Tac_list_C;

		Leaf_Tac* L_Opd = this->cond_part->insertTac(Tac_list_C);
		this->body_part->insertTac(Tac_list_B);

		string new_label0 = "Label"+to_string(label_index);
		label_index++;

		// add Label0:
		Label_Tac* newlabel_node  = new Label_Tac(new_label0);
		Tac_list.push_back(newlabel_node);

		Tac_list.insert(Tac_list.end(), Tac_list_B.begin(), Tac_list_B.end());
		Tac_list.insert(Tac_list.end(), Tac_list_C.begin(), Tac_list_C.end());

		// add if(temp0) goto label0;
		Control_flow_Tac* newgoto = new Control_flow_Tac(L_Opd->fn_data(), new_label0);
		Tac_list.push_back(newgoto);

		return L_Opd;	
	}
	else{
		// add Label1:
		Label_Tac* newlabel_node  = new Label_Tac("Labelxxx");
		Tac_list.push_back(newlabel_node);
		int lblindx = Tac_list.size()-1;

		Leaf_Tac* L_Opd = this->cond_part->insertTac(Tac_list);
		
		vector<Tac_Node*> Tac_list_T;

		this->body_part->insertTac(Tac_list_T);

		string new_label0 = "Label"+to_string(label_index);
		label_index++;
		string new_label1 = "Label"+to_string(label_index);
		label_index++;

		Tac_list[lblindx] =  new Label_Tac(new_label0);

		// add negation of cond
		string new_temp = "temp"+to_string(temp_index);
		temp_index++;
		Name_Tac* newname = new  Name_Tac(new_temp);
		Compute_Tac* newnode = new Compute_Tac(newname, Not_E,L_Opd, NULL);

		Tac_list.push_back(newnode);

		// add if(temp0) goto label0;
		Control_flow_Tac* newgoto = new Control_flow_Tac(new_temp, new_label1);
		
		Tac_list.push_back(newgoto);
		
		// add true part tac list
		Tac_list.insert(Tac_list.end(), Tac_list_T.begin(), Tac_list_T.end());

		// add goto Label1
		newgoto = new Control_flow_Tac("", new_label0);
		Tac_list.push_back(newgoto);

		// add Label0:
		newlabel_node  = new Label_Tac(new_label1);
		Tac_list.push_back(newlabel_node);

		return newname;	
	}
}

Leaf_Tac* If_Else_Ast::insertTac(vector<Tac_Node*> &Tac_list){
	Leaf_Tac* L_Opd = this->cond_part->insertTac(Tac_list);
	
	vector<Tac_Node*> Tac_list_T;
	vector<Tac_Node*> Tac_list_F;

	Leaf_Tac* true_result = this->true_Part->insertTac(Tac_list_T);
	
	// add negation of cond
	string new_temp = "temp"+to_string(temp_index);
	temp_index++;
	Name_Tac* newname = new  Name_Tac(new_temp);
	Compute_Tac* newnode = new Compute_Tac(newname, Not_E,L_Opd, NULL);

	Tac_list.push_back(newnode);


	string new_label0 = "Label"+to_string(label_index);
	label_index++;
	string new_label1;
	if(this->false_Part != NULL){ 
		new_label1 = "Label"+to_string(label_index);
		label_index++;
	}

	if(this->false_Part != NULL){ 
		Leaf_Tac* false_result = this->false_Part->insertTac(Tac_list_F);
	}

	// add if(temp0) goto label0;
	Control_flow_Tac* newgoto;
	if(this->false_Part != NULL){ 
		newgoto = new Control_flow_Tac(new_temp, new_label1);
	}
	else{
		newgoto = new Control_flow_Tac(new_temp, new_label0);
	}

	Tac_list.push_back(newgoto);
	
	// add true part tac list
	Tac_list.insert(Tac_list.end(), Tac_list_T.begin(), Tac_list_T.end());

	// add goto Label1
	newgoto = new Control_flow_Tac("", new_label0);
	Tac_list.push_back(newgoto);

	Label_Tac* newlabel_node;
	if(this->false_Part != NULL){ 
		// add Label0:
		newlabel_node  = new Label_Tac(new_label1);
		Tac_list.push_back(newlabel_node);

		// add false part tac list
		Tac_list.insert(Tac_list.end(), Tac_list_F.begin(), Tac_list_F.end());
	}

	// add Label1:
	newlabel_node  = new Label_Tac(new_label0);
	Tac_list.push_back(newlabel_node);

	return L_Opd;		
}


Leaf_Tac* Return_Ast::insertTac(vector<Tac_Node*> &Tac_list){

}
Leaf_Tac* Fun_call_Ast::insertTac(vector<Tac_Node*> &Tac_list){
	
}
