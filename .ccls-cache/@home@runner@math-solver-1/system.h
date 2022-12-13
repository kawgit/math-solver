#pragma once
#include "node.h"
#include <string>

using namespace std;

enum Relation_Type { EQU, GOE, GRT, LOE, LES };

extern map<Relation_Type, string> rel_str;

struct Equation {
    Relation_Type rel_type = EQU;
    const Node* child1 = nullptr;
    const Node* child2 = nullptr;
    Equation() {}
    Equation(const Node* c1, Relation_Type rel, const Node* c2) : child1{c1} , child2{c2} , rel_type{rel} {}
};

string to_string(Equation equ);

struct System {
    static vector<Equation> identities;
    vector<Equation> equations;
    const Node* get_value(const Leaf* leaf);
};
