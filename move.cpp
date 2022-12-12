#include "node.h"
#include "node_manip.h"
#include "comp.h"
#include "system.h"
#include "move.h"
#include <cassert>
#include <iostream>

using namespace std;

string to_string(Move move) {
    return "MOVE {" + to_string(move.root_path) + "," + to_string(move.identity) + "}";
}

Node* do_move(const Node* node, const Move move) {
    const Node* sub_root = fetch(node, move.root_path);
    const Node* replacement = identity_sub(move.identity, sub_root);

    if (!replacement) return nullptr;

    Node* result = replace_node(node, replacement, move.root_path);

    assert(result->type != ROOT || TO_CRP(result)->children.size());

    return result;
}

Node* replace_node(const Node* node, const Node* insert, const vector<int>& path) { 

    if (path.size() == 0) {
        return insert->clone();
    }

    assert(node->type == ROOT);
	assert(TO_CRP(node)->children.size() != 0);

    vector<const Root*> node_path;
    node_path.push_back(TO_CRP(node));
    
    const Root* curr = TO_CRP(node);
    for (int i = path.size() - 1; i >= 1; i--) {
        curr = TO_CRP(TO_CRP(curr)->children[path[i]]);
        assert(curr->type == ROOT);
	    assert(TO_CRP(curr)->children.size() != 0);
        node_path.push_back(curr);
    }

    // node_path.back() == to the parent of the node we are trying to replace

    Node* result = insert->clone();
    for (int i = node_path.size() - 1; i >= 0; i--) {
        result = replace_child(node_path[i], path[path.size() - 1 - i], result);
    }

    return result;
}

vector<vector<int>> get_paths(const Node* node) {
    if (node->type == LEAF) return {{}};

    assert(node->type == ROOT);
	assert(TO_CRP(node)->children.size() != 0);

    const Root* cast_node = TO_CRP(node);

    vector<vector<int>> paths = {{}};
    for (int i = 0; i < cast_node->children.size(); i++) {
        paths.push_back({i});
        const Node* child = cast_node->children[i];
        if (child->type == ROOT) {
	        assert(TO_CRP(child)->children.size() != 0);


            vector<vector<int>> child_paths = get_paths(child);
            for (vector<int>& path : child_paths) {
                path.push_back(i);
                paths.push_back(path);
            }
        }
    }

    return paths;
}

const Node* fetch(const Node* node, const vector<int>& path) {
    const Node* curr = node;
    for (int i = path.size() - 1; i >= 0; i--) {
        assert(curr->type == ROOT);
    	assert(TO_CRP(node)->children.size() != 0);

        const Root* cast_curr = TO_CRP(curr);
        curr = cast_curr->children[path[i]];
    }
    return curr;
}

string to_string(vector<int> path) {
    string str = "";
    for (int& i : path) {
        str += to_string(i);
    }
    return str;
}