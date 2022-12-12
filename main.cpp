#include <iostream>
#include "node.h"
#include "node_manip.h"
#include "parse.h"
#include "system.h"
#include "move.h"
#include "identities.h"
#include "search.h"

using namespace std;

int main() {
	Node::init_storage();
	load_identities("identities.txt");

	string str = "(x+2)*(x+3+3+3-10*x)=0";
	Equation equ = to_equation(str);
	cout << "main:" << to_string(equ) << endl;

	System sys;
	equ.child1 = arith_clean(equ.child1);

	cout << to_string(equ.child1) << endl;

	Search_Result sr = simp_search(sys, equ, 3);
	cout << "main:" << to_string(sr) << endl;
	cout << "final:" << to_string(sr.leaf) << endl;
	cout << "end " << to_string(equ.child1) << endl;

	Node* curr = equ.child1->clone();
	cout << to_string(curr) << endl;
	for (int i = sr.moves.size() - 1; i >= 0; i--) {
		curr = arith_clean(do_move(curr, sr.moves[i]));
		cout << to_string(curr) << endl;
	}


	cout << "main exiting." << endl;
	cout << "roots: " << Root::node_storage.size() << endl;
	cout << "leafs: " << Leaf::node_storage.size() << endl;
}