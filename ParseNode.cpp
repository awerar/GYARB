#include "ParseNode.h"

ParseNode::ParseNode(Token token)
{
	this->token = token;
}

ParseNode::~ParseNode()
{
	for (auto* child : children)
	{
		delete child;
	}
}
