#pragma once 
#include "node.h"
#include "system.h"
#include "move.h"

struct Search_Result {
    Equation leaf;
    int score = 0;
    vector<Move> moves = {};
    
    Search_Result() {}
    Search_Result(Equation init_leaf, int init_score, vector<Move> init_moves) : leaf{init_leaf}, score{init_score}, moves{init_moves} {}
};

string to_string(Search_Result sr);

Search_Result simp_search(System& sys, Equation equ, int depth);

// void solve_for_search(Equation equ, int depth, const Leaf* target);