#pragma once

#include <vector>
#include "system.h"

using namespace std;

struct Move {
    vector<int> root_path;
    Equation identity;

    Move() {}
    Move(vector<int> init_root_path, Equation init_identity) : root_path{init_root_path}, identity{init_identity} {}
};

string to_string(Move move);

Node* do_move(const Node* node, const Move move);

Node* replace_node(const Node* node, const Node* insert, const vector<int>& path);

vector<vector<int>> get_paths(const Node* node);

const Node* fetch(const Node* node, const vector<int>& path);

template<typename T>
string to_string(vector<T> vec ) {
    string str = "LIST {";
    for (T& t : vec) {
        str += to_string(t) + " | ";
    }
    return str + "}";
}