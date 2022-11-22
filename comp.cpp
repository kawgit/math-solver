#include "comp.h"
#include "node_manip.h"
#include <vector>
#include <cassert>
#include <math.h>

using namespace std;

Leaf* compute(Root_Type rt, const vector<const Leaf*> params) {
	assert(is_arith(params));
	switch (rt) {
		case ADD: {
			double result = 0;
			for (const Leaf* param : params) result += param->value;
			return Leaf::make_leaf(result);
		}
		case NEG:
			assert(params.size() == 1);
			return Leaf::make_leaf(-params[0]->value);
		case MUL: {
			double result = 1;
			for (const Leaf* param : params) result *= param->value;
			return Leaf::make_leaf(result);
		}
		case DIV:
			assert(params.size() == 2);
			return Leaf::make_leaf(params[0]->value / params[1]->value);
		case POW:
			assert(params.size() == 2);
			return Leaf::make_leaf(pow(params[0]->value, params[1]->value));
		case SIN:
			assert(params.size() == 1);
			return Leaf::make_leaf(sin(params[0]->value));
		case COS:
			assert(params.size() == 1);
			return Leaf::make_leaf(cos(params[0]->value));
		case TAN:
			assert(params.size() == 1);
			return Leaf::make_leaf(tan(params[0]->value));

	}
	assert(false);
}