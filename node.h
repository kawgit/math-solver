#pragma once

#include <vector>
#include <stdint.h>
#include <string>
#include <cassert>
#include <map>

#define TO_CLP(x) static_cast<const Leaf*>(x)
#define TO_LP(x) static_cast<Leaf*>(x)
#define TO_CRP(x) static_cast<const Root*>(x)
#define TO_RP(x) static_cast<Root*>(x)

using namespace std;

typedef uint64_t Hash;
enum Node_Type {ROOT, LEAF};
enum Leaf_Type {NUMBER, VARIABLE, CONSTANT};
enum Root_Type {ADD, NEG, MUL, DIV, POW, SIN, COS, TAN};

extern map<Root_Type, int> rt_operands;
extern map<Root_Type, string> rt_str;
extern map<Leaf_Type, string> lt_str;

class Node {
	public:
		Node_Type type;
		static void init_storage();
		virtual Node* clone() const { return nullptr; };
		
};

string to_string(const Node* node);

class Root : public Node {
	friend Node;

	public:
		Root(Root_Type rt_) {	
			type = ROOT;
			rt = rt_;
		}
		
		static Root* make_root(Root_Type rt_);
		static vector<Root> node_storage;

	public:
		Root_Type rt = ADD;
		vector<const Node*> children;

		void add_leaf(double value_);
		void add_leaf(string var_name, Leaf_Type lt_);
		void add_root(Root_Type rt_);
		void add_children(const Root* node);
		void add_children_but_zeroth(const Root* node);
		const Node* get_last_child() const;
		Node* clone() const;
};

class Leaf : public Node {
	friend Root;
	friend Node;

	public:
		Leaf(double value_) {
			type = LEAF;
			lt = NUMBER;
			value = value_;
		}

		Leaf(string var_name, Leaf_Type lt_) {
			type = LEAF;
			lt = lt_;
			assert(lt == VARIABLE || lt == CONSTANT);
			if (name_to_id.find(var_name) == name_to_id.end()) {
				name_to_id.insert({var_name, rand()});
				id_to_name.insert({name_to_id.at(var_name), var_name});
			}
			value = name_to_id.at(var_name);
		}

		static Leaf* make_leaf(double value_);
		static Leaf* make_leaf(string var_name, Leaf_Type lt_);

		static map<double, string> id_to_name;
		static vector<Leaf> node_storage;
	private:
		static map<string, double> name_to_id;
		
	public:
		Leaf_Type lt = NUMBER;
		double value;

		Node* clone() const;
};

const static size_t RESERVE_SIZE = 100000000;
const static size_t MB_MEM_NEEDED = (RESERVE_SIZE*sizeof(Leaf) + RESERVE_SIZE*sizeof(Root))/1000000;