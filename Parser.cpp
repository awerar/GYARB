#include "Parser.h"

//https://en.wikipedia.org/wiki/LR_parser#Bottom-up_parse_tree_for_example_A*2_+_1

Parser::Parser(Lexer* lexer) : table(grammar)
{
	this->lexer = lexer;
}

Parser::~Parser()
{
	delete lexer;
}