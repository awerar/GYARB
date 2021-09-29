#include "ParserStack.h"
#include <iostream>

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

void print_parse_tree(ParseNode* tree, int depth)
{
	if (tree == NULL) {
		std::cout << "EMPTY";
		return;
	}

	for (int i = 0; i < depth; i++) std::cout << "---";
	std::cout << get_token_name(tree->token) << "\n";

	for (auto child : tree->children) print_parse_tree(child, depth + 1);
}

void print_parse_stack(ParseStack stack) {
	while (!stack.empty()) {
		print_parse_tree(stack.top().second);
		std::cout << "\n";

		stack.pop();
	}
}