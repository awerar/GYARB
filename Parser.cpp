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
}
