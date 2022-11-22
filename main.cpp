#include <iostream>
#include "node.h"
#include "node_manip.h"
#include "parse.h"
#include "system.h"

using namespace std;

int main() {
	Node::init_storage(1000);
	
	string str = "10*x^2+2*x+-100=0";
	Equation equ = to_equation(str);
	cout << "main:" << to_string(equ) << endl;
	Equation identity = to_equation("a*x^2+b*x+c=(x+-(-b+(b^2+-4*a*c)^(1/2))/(2*a))*(x+-(-b+-(b^2+-4*a*c)^(1/2))/(2*a))");
	cout << "main:" << to_string(identity) << endl;
	const Node* result = identity_sub(identity, equ.child1);
	cout << "main:" << to_string(result) << endl;
	cout << "main:" << to_string(arith_clean(result)) << endl;
}