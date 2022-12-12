#include "parse.h"
#include "node.h"
#include "system.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <map>

using namespace std;

string remove_char(string str, char target) {
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
	return str;
}

map<string, Root_Type> str_rt = {
	{"+", ADD},
	{"-", NEG},
	{"*", MUL},
	{"/", DIV},
	{"^", POW},
	{"sin", SIN},
	{"cos", COS},
	{"tan", TAN},
};

map<string, Relation_Type> str_rel = {
	{"=", EQU},
	{">=", GOE},
	{">", GRT},
	{"<=", LOE},
	{"<", LES},
};

int get_precedence(Token token) {
	switch (token.tt) {
		case NUMBER_TOKEN:
			assert(false);
		case FUNCTION_TOKEN:
			switch (str_rt[token.str]) {
				case ADD:
					return 1;
				case MUL:
				case DIV:
					return 2;
				case NEG:
					return 3;
				case POW:
					return 4;
				case SIN:
				case COS:
				case TAN:
					return 5;
			}
		case LP_TOKEN:
			return -1;
		case RP_TOKEN:
			return -1;
		case RELATION_TOKEN:
			return -2;
	}
	assert(false);
	return 0;
}

Token peek_token(string expr_str) {
	if (expr_str[0] >= '0' && expr_str[0] <= '9') {
		int i = 0;
		while (expr_str[i] >= '0' && expr_str[i] <= '9') {
			i++;
		}
		return Token(NUMBER_TOKEN, expr_str.substr(0, i));
	}
	
	for (auto itr = str_rt.begin(); itr != str_rt.end(); itr++) {
		if (expr_str.rfind(itr->first, 0) != string::npos) {
			return Token(FUNCTION_TOKEN, expr_str.substr(0, itr->first.size()));
		}
	}

	for (auto itr = str_rel.begin(); itr != str_rel.end(); itr++) {
		if (expr_str.rfind(itr->first, 0) != string::npos) {
			return Token(RELATION_TOKEN, expr_str.substr(0, itr->first.size()));
		}
	}
	
	switch (expr_str[0]) {
		case '(':
			return Token(LP_TOKEN, expr_str.substr(0, 1));
		case ')':
			return Token(RP_TOKEN, expr_str.substr(0, 1));
		default:
			return Token(NUMBER_TOKEN, expr_str.substr(0, 1));
	}
}

Token pop_token(string& expr_str) {
	Token token = peek_token(expr_str);
	expr_str = expr_str.substr(token.str.size());
	return token;
}

string to_string(const vector<Token>& tokens) {
	string result = "LIST {";
	for (Token token : tokens) {
		result += token.str + " ";
	}
	return result + "}";
}

vector<Token> tokenize(string expr_str) {
	expr_str = remove_char(expr_str, ' ');
	expr_str = remove_char(expr_str, ',');
	expr_str = remove_char(expr_str, '\n');
	expr_str = remove_char(expr_str, '\r');

	// a-b -> a+-b

	for (int i = expr_str.size() - 1; i >= 1; i--) {
		if (expr_str[i] == '-') {
			switch (expr_str[i - 1]) {
				case '+':
				case '-':
				case '*':
				case '/':
				case '^':
				case '(':
					break;
				default:
					expr_str = expr_str.substr(0, i) + '+' + expr_str.substr(i);
					break;
			}
		}
	}

	vector<Token> op_stack;
	vector<Token> out_queue;
	
	while (expr_str.size()) {
		Token token = pop_token(expr_str);
		switch (token.tt) {
			case NUMBER_TOKEN:
				out_queue.push_back(token);
				break;
			case RELATION_TOKEN:
			case FUNCTION_TOKEN:
				while (op_stack.size() && get_precedence(op_stack.back()) >= get_precedence(token) && rt_operands[str_rt[token.str]] == 2) {
					out_queue.push_back(op_stack.back());
					op_stack.pop_back();
				}
				op_stack.push_back(token);
				break;
			case LP_TOKEN:
				op_stack.push_back(token);
				break;
			case RP_TOKEN:
				while (op_stack.size() && op_stack.back().tt != LP_TOKEN) {
					out_queue.push_back(op_stack.back());
					op_stack.pop_back();
				}
				op_stack.pop_back();
				break;
				
			default:
				assert(false);
		}
	}

	while (op_stack.size()) {
		out_queue.push_back(op_stack.back());
		op_stack.pop_back();
	}

	return out_queue;
}

Node* to_node(const vector<Token>& tokens) {
	assert(tokens.size());
	assert(tokens.back().tt != RELATION_TOKEN);

	if (tokens.back().tt == NUMBER_TOKEN) {
		if (tokens.back().str[0] >= '0' && tokens.back().str[0] <= '9') {
			return Leaf::make_leaf(stod(tokens.back().str));
		}
		else {
			return Leaf::make_leaf(tokens.back().str, VARIABLE);
		}
	}

	assert(tokens.back().tt == FUNCTION_TOKEN);

	Root_Type rt = str_rt[tokens.back().str];
	vector<const Node*> children;
	int needed_operands = 1;
	int greatest_operator_index = tokens.size() - 1;
	for (int i = tokens.size() - 2; i >= 0; i--) {
		Token token = tokens[i];
		switch (token.tt) {
			case NUMBER_TOKEN:
				needed_operands--;
			break;
			case FUNCTION_TOKEN:
				needed_operands += rt_operands[str_rt[token.str]] - 1;
			break;
			case LP_TOKEN:
			case RP_TOKEN:
			case RELATION_TOKEN:
				assert(false);
		}

		if (needed_operands == 0) {
			const vector<Token> subset(tokens.begin() + i, tokens.begin() + greatest_operator_index);
			children.insert(children.begin(), to_node(subset));
			greatest_operator_index = i;
			needed_operands = 1;
		}
	}

	Root* result = Root::make_root(rt);
	result->children = children;
	return result;
}

Equation to_equation(string str) {

	int rel_token_index = -1;
	Relation_Type rel_token_type;
	for (auto itr = str_rel.begin(); itr != str_rel.end(); itr++) {
		if (str.find(itr->first) != string::npos) {
			rel_token_index = str.find(itr->first);
			rel_token_type = itr->second;
		}
	}
	assert(rel_token_index != -1);

	return Equation(to_node(tokenize(str.substr(0, rel_token_index)))
		, rel_token_type
		, to_node(tokenize(str.substr(rel_token_index + 1))));
}
