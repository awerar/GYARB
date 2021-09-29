#pragma once

#include "ParserStack.h"
#include <vector>
#include "Grammar.h"
#include <stack>

struct ParseNode
{
	Token token;
	std::vector<ParseNode*> children;

	ParseNode(Token token);
	~ParseNode();
};

typedef std::pair<int, ParseNode*> ParseStackFrame;
typedef std::stack<ParseStackFrame> ParseStack;

void print_parse_tree(ParseNode* tree, int depth = 0);
void print_parse_stack(ParseStack stack);
