#include "Parser.h"
#include "Error.cpp"
#include <iostream>

//https://en.wikipedia.org/wiki/LR_parser#Bottom-up_parse_tree_for_example_A*2_+_1

Parser::Parser(Lexer* lexer)
{
	this->lexer = lexer;
	this->result = nullptr;
}

Parser::~Parser()
{
	delete lexer;
}

ParseNode* Parser::parse()
{
	if (result != nullptr) return result;

	ParseStack* stack = new ParseStack({ ParseStackFrame(0, nullptr) });

	try {
		while (!table.perform(stack, lexer));
	}
	catch (SyntaxError e) {
		std::cout << e << std::endl;
		std::cout << "DUMP OF PARSE STACK:\n";
		print_parse_stack(*stack);

		throw e;
	}

	result = stack->top().second;
	delete stack;
	
	return result;
}