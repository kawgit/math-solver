#include "system.h"
#include "search.h"
#include "eval.h"
#include "move.h"
#include "identities.h"
#include "node_manip.h"
#include <iostream>

using namespace std;

string to_string(Search_Result sr) {
    return "SR {" + to_string(sr.leaf) + "," + to_string(sr.score) + ", list of moves: " + to_string(sr.moves) + " }";
}

Search_Result simp_search(System& sys, Equation equ, int depth) {
    if (depth <= 0) return Search_Result(equ, simp_eval(equ), {});

    vector<vector<int>> paths = get_paths(equ.child1);

    const Node* node = equ.child1;
    Move best_move;
    Search_Result best_result(equ, simp_eval(equ), {});

    for (vector<int>& path : paths) {
        for (Equation identity : identities) {
            Move move = Move(path, identity);
            Node* altered_node = do_move(node, move);

            if (altered_node) {

                altered_node = arith_clean(altered_node);

                // if (depth == 3) cout << to_string(node) << "move: " << to_string(move) << " altered: " << to_string(altered_node) << endl;

                Search_Result search_result = simp_search(sys, Equation(altered_node, EQU, equ.child2), depth - 1);
                
                if (search_result.score < best_result.score) {
                    best_move = move;
                    best_result = search_result;
                    best_result.moves.push_back(best_move);
                }
            }
        }
    }

    return best_result;
}