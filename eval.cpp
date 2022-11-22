#include "stdint.h"
#include "system.h"
#include "node.h"
#include "eval.h"

int count_funcs(const Node* node) {
    if (node->type == LEAF) return 1;
    int count = 0;
    for (const Node* child : TO_CRP(node)->children) {
        count += count_funcs(child);
    }
    return count;
}

Eval simp_eval(Equation& equ) {
    Eval score = 0;
    if (equ.child1->type == LEAF) {
        score -= 10000;
        if (TO_CLP(equ.child1)->value == 0) {
            score -= 1000000;
        }
    }
    if (equ.child2->type == LEAF) {
        score -= 10000;
        if (TO_CLP(equ.child2)->value == 0) {
            score -= 1000000;
        }
    }

    score += count_funcs(equ.child1);
    score += count_funcs(equ.child2);
    
    return score;
}