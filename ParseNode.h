#pragma once

#include <vector>
#include "Token.h"

struct ParseNode
{
	Token token;
	std::vector<ParseNode*> children;

	ParseNode(Token token);
	~ParseNode();
};