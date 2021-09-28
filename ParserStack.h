#pragma once

#include "ParserStack.h"
#include <vector>
#include "Token.h"
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