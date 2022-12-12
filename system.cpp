#include "system.h"
#include <vector>

using namespace std;

map<Relation_Type, string> rel_str = {
	{EQU, "="},
	{GOE, ">="},
	{GRT, ">"},
	{LOE, "<="},
	{LES, "<"},
};

string to_string(Equation equ) {
    return "EQU {" + to_string(equ.child1) + " " + rel_str[equ.rel_type] + " " + to_string(equ.child2) + "}";
}

const Node* System::get_value(const Leaf* leaf) {
    for (Equation& equ : equations) {
        if (equ.child1->type == LEAF 
            && TO_CLP(equ.child1)->lt != NUMBER 
            && TO_CLP(equ.child1)->value == leaf->value) {
            return equ.child2;
        }
    }
    return nullptr;
}