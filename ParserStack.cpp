#include "ParserStack.h"
#include <iostream>

ParseNode::ParseNode(Token token, std::string text)
{
	this->token = token;
	this->text = text;
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

	std::string msg = "";
	for (int i = 0; i < depth; i++) msg += "---";
	msg += get_token_name(tree->token);
	std::cout << msg;

	for (int i = 50; i > msg.size(); i--) std::cout << " ";

	if (tree->text != "") std::cout << "<" << tree->text << ">";
	std::cout << "\n";

	for (auto child : tree->children) print_parse_tree(child, depth + 1);
}

void print_parse_stack(ParseStack stack) {
	while (!stack.empty()) {
		print_parse_tree(stack.top().second);
		std::cout << "\n";

		stack.pop();
	}
}