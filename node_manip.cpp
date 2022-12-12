#include "node.h"
#include "node_manip.h"
#include "comp.h"
#include "system.h"
#include <cassert>
#include <cmath>
#include <iostream>

bool is_arith(const Node* node) {
	if (node->type == LEAF) {
		return TO_CLP(node)->lt == NUMBER;
	}
	for (const Node* child : TO_CRP(node)->children) {
		if (!is_arith(child)) {
			return false;
		}
	}
	return true;
}

bool is_arith(const std::vector<const Node*> nodes) {
	for (const Node* node : nodes) {
		if (!is_arith(node)) {
			return false;
		}
	}
	return true;
}

bool is_arith(const std::vector<const Leaf*> nodes) {
	for (const Node* node : nodes) {
		if (!is_arith(node)) {
			return false;
		}
	}
	return true;
}

Leaf* arith_simplify(const Node* node) {
	assert(is_arith(node));
	if (node->type == LEAF) {
		assert(node->clone() != nullptr);
		return TO_LP(node->clone());
	}

	const Root* cast_node = TO_CRP(node);
	vector<const Leaf*> simp_children;
	simp_children.reserve(cast_node->children.size());
	for (const Node* child : cast_node->children) {
		simp_children.push_back(arith_simplify(child));
	}
	return compute(cast_node->rt, simp_children);
}

Node* arith_clean(const Node* node) {
	assert(node);

	if (is_arith(node)) {
		return arith_simplify(node);
	}

	switch (node->type) {
		case LEAF:
			return node->clone();
		case ROOT:
			const Root* cast_node = TO_CRP(node);
			Root* result = Root::make_root(cast_node->rt);
			for (const Node* child : cast_node->children) {
				result->children.push_back(arith_clean(child));
			}
			return result;
	}
}

bool identical(const Node* node1, const Node* node2) {
	if (node1->type != node2->type) return false;
	switch (node1->type) {
		case LEAF: {
			const Leaf* cast_node1 = TO_CLP(node1);
			const Leaf* cast_node2 = TO_CLP(node2);
			return cast_node1->lt == cast_node2->lt
				&& cast_node1->value == cast_node2->value;
		}
		case ROOT: {
			const Root* cast_node1 = TO_CRP(node1);
			const Root* cast_node2 = TO_CRP(node2);
			if (cast_node1->rt != cast_node2->rt) return false;
			if (cast_node1->children.size() != cast_node2->children.size()) return false;
			for (int i = 0; i < cast_node1->children.size(); i++) {
				if (!identical(cast_node1->children[i], cast_node2->children[i])) {
					return false;
				}
			}
			return true;
		}
			
	}
}

void var_sub(const Equation equ, Node*& target) {
	assert(equ.child1->type == LEAF);
	switch (target->type) {
		case LEAF:
			if (TO_LP(target)->lt == TO_CLP(equ.child1)->lt
				&& TO_LP(target)->value == TO_CLP(equ.child1)->value) {
					target = equ.child2->clone();
			}
			break;
		case ROOT:
			Root* cast_target = TO_RP(target);
			Root* target_replacement = Root::make_root(cast_target->rt);
			for (const Node* child : cast_target->children) {
				Node* clone = child->clone();
				var_sub(equ, clone);
				target_replacement->children.push_back(clone);
			}
			target = target_replacement;
			break;
	}
}

bool check_identity_sub(System& sys, const Node* plan, const Node* target) {
	switch (plan->type) {
		case LEAF: {
			if (TO_CLP(plan)->lt != NUMBER) {
				const Node* value = sys.get_value(TO_CLP(plan));
				if (!value) {
					sys.equations.emplace_back(plan, EQU, target);
					return true;
				}
				else {
					return identical(value, target); 
				}
			}
			else {
				return TO_CLP(target)->lt == NUMBER && TO_CLP(plan)->value == TO_CLP(target)->value;
			}
		}
		case ROOT: {
			if (target->type != ROOT) return false;

			const Root* cast_plan = TO_CRP(plan);
			const Root* cast_target = TO_CRP(target);
			if (cast_plan->rt != cast_target->rt) return false;
			if (cast_plan->children.size() != cast_target->children.size()) return false;
			for (int i = 0; i < cast_plan->children.size(); i++) {
				if (!check_identity_sub(sys, cast_plan->children[i], cast_target->children[i])) return false;
			}
			return true;
		}
	}
}

Node* identity_sub(const Equation identity, const Node* target) {
	System sys;
	if (!check_identity_sub(sys, identity.child1, target)) {
		return nullptr;
	}
	Node* result = identity.child2->clone();
	for (Equation& equ : sys.equations) {
		var_sub(equ, result);
	}
	return result;
}


Root* replace_child(const Node* node, int target_index, const Node* child) {
	assert(node->type == ROOT);
	assert(TO_CRP(node)->children.size() != 0);
	const Root* cast = TO_CRP(node);

	Root* result = Root::make_root(cast->rt);
	for (int i = 0; i < cast->children.size(); i++) {
		if (i == target_index) {
			result->children.push_back(child);
		}
		else {
			result->children.push_back(cast->children[i]);
		}
	}

	return result;
}

Node* remove_associative_duplicates(const Node* node) {
	return nullptr;
}