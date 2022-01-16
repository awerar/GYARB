#include "ASTNode.h"
#include "InterpreterLambda.h"
#include <iostream>

#pragma once
class Interpreter
{
private:
	ASTNode* ast;
	int symbols_used;

	InterpreterLambda* run(ASTNode* ast);
	int get_symbols_used(ASTNode* ast);

public:
	std::istream* input;
	std::ostream* output;

	Interpreter(ASTNode* ast, std::istream* input, std::ostream* output);
	void run();
	int get_new_symbol();
};