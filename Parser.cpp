#include "Parser.h"
#include <iostream>

//https://en.wikipedia.org/wiki/LR_parser#Bottom-up_parse_tree_for_example_A*2_+_1

Parser::Parser(Lexer* lexer)
{
	this->lexer = lexer;
	this->result = NULL;
	std::cout << table;
}

Parser::~Parser()
{
	delete lexer;
}

ParseNode* Parser::parse()
{
	if (result != NULL) return result;

	ParseStack* stack = new ParseStack({ ParseStackFrame(0, NULL) });
	while (!table.perform(stack, lexer));

	result = stack->top().second;
	delete stack;
	
	return result;
}

void Parser::print_parse_tree(ParseNode* tree, int depth)
{
	for (int i = 0; i < depth; i++) std::cout << "---";
	std::cout << get_token_name(tree->token) << "\n";

	for (auto child : tree->children) print_parse_tree(child, depth + 1);
}

