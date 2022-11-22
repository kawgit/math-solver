#include "node.h"
#include <string>
#include <iostream>
#include <map>

using namespace std;

vector<Leaf> Leaf::node_storage = {};
vector<Root> Root::node_storage = {};
map<string, double> Leaf::name_to_id;
map<double, string> Leaf::id_to_name;

map<Root_Type, int> rt_operands = {
	{ADD, 2},
	{NEG, 1},
	{MUL, 2},
	{DIV, 2},
	{POW, 2},
	{SIN, 1},
	{COS, 1},
	{TAN, 1},
};

map<Root_Type, string> rt_str = {
	{ADD, "ADD"},
	{NEG, "NEG"},
	{MUL, "MUL"},
	{DIV, "DIV"},
	{POW, "POW"},
	{SIN, "SIN"},
	{COS, "COS"},
	{TAN, "TAN"},
};

map<Leaf_Type, string> lt_str = {
	{NUMBER, "NUMBER"},
	{VARIABLE, "VARIABLE"},
	{CONSTANT, "CONSTANT"},
};

string to_string(const Node* node) {
	switch (node->type) {
		case LEAF: {
			const Leaf* cast_node = TO_CLP(node);
			return (cast_node->lt == NUMBER ? to_string(cast_node->value) : Leaf::id_to_name[cast_node->value]);
		}
		case ROOT: {
			const Root* cast_node = TO_CRP(node);
			string result = rt_str[cast_node->rt] + "(";
			for (int i = 0; i < cast_node->children.size() - 1; i++) {
				result += to_string(cast_node->children[i]) + ",";
			}
			return result + to_string(cast_node->children.back()) + ")";
		}
	}
}

void Node::init_storage(int reserve_size) {
	Root::node_storage.reserve(reserve_size);
	Leaf::node_storage.reserve(reserve_size);
}

Root* Root::make_root(Root_Type rt_) {
	Root::node_storage.emplace_back(rt_);
	return &Root::node_storage.back();
}

void Root::add_leaf(double value_) {
	children.push_back(Leaf::make_leaf(value_));
}

void Root::add_leaf(string var_name, Leaf_Type lt_) {
	children.push_back(Leaf::make_leaf(var_name, lt_));
}

void Root::add_root(Root_Type rt_) {
	children.push_back(Root::make_root(rt_));
}

void Root::add_children(const Root* node) {
	for (const Node* child : node->children) {
		children.push_back(child);
	}
}

void Root::add_children_but_zeroth(const Root* node) {
	for (int i = 1; i < node->children.size(); i++) {
		children.push_back(node->children[i]);
	}
}

const Node* Root::get_last_child() const {
	return children.back();
}

Node* Root::clone() const {
	Root* root = Root::make_root(rt);
	root->children = children;
	return root;
}

Leaf* Leaf::make_leaf(double value_) {
	Leaf::node_storage.emplace_back(value_);
	return &Leaf::node_storage.back();
}

Leaf* Leaf::make_leaf(string var_name, Leaf_Type lt_) {
	Leaf::node_storage.emplace_back(var_name, lt_);
	return &Leaf::node_storage.back();
}

Node* Leaf::clone() const {
	return lt == NUMBER ? Leaf::make_leaf(value) : Leaf::make_leaf(id_to_name.at(value), lt);
}
