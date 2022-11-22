#pragma once

#include "node.h"
#include "system.h"
#include <string>

enum Token_Type {NUMBER_TOKEN, FUNCTION_TOKEN, LP_TOKEN, RP_TOKEN, RELATION_TOKEN};

struct Token {
	Token_Type tt;
	string str;
	Token(Token_Type tt_, string str_) : tt(tt_), str(str_) {}
};

string remove_char(string str, char target);

Token peek_token(string expr_str);

Token pop_token(string& expr_str);

string to_string(const vector<Token>& tokens);

vector<Token> tokenize(string expr_str);

Node* to_node(const vector<Token>& tokens);

Equation to_equation(string str);