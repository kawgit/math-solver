#include "identities.h"
#include "system.h"
#include "node.h"
#include "parse.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

vector<Equation> identities;

vector<char> unused_chars = {(char)224, (char)225, (char)226, (char)229, (char)230, (char)231, (char)232, (char)237, '#', '$', '&'};

void replace_common_symbols(string& line) {
    string copy = line;
    int unused_chars_itr = 0;
    while (copy.size()) {
        Token token = pop_token(copy);
        if (token.tt == NUMBER_TOKEN && ((token.str[0] >= 'a' && token.str[0] <= 'z') || (token.str[0] >= 'A' && token.str[0] <= 'Z'))) {
            replace(line.begin(), line.end(), token.str[0], unused_chars[unused_chars_itr]);
            unused_chars_itr++;

            assert(unused_chars_itr < unused_chars.size());
        }
    }
}

void load_identities(string path) {
    ifstream file(path);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            replace_common_symbols(line);
            identities.emplace_back(to_equation(line));
            cout << "loaded identity: " + line + " -> " + to_string(identities.back()) << endl;
        }
    }
    else {
        cout << "could not open file " << path << endl;
    }
    cout << "finished loading identities" << endl;
}

