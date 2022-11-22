#pragma once

#include "node.h"
#include "system.h"
#include <vector>


bool is_arith(const Node* node);

bool is_arith(const std::vector<const Node*> nodes);

bool is_arith(const std::vector<const Leaf*> nodes);

// must be all arith (no leafs other than type NUMBER)
Leaf* arith_simplify(const Node* node);

// will simplify all arith nested ROOT (can have types other than NUMBER) 
Node* arith_clean(const Node* node);

bool identical(const Node* node1, const Node* node2);

Node* identity_sub(const Equation identity, const Node* target);

Hash get_hash(const Node* node);
