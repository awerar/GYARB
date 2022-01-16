#include "Interpreter.h"
#include <iostream>
#include "Error.cpp"

InterpreterLambda* Interpreter::run(ASTNode* ast)
{
	if (ast->type == ASTNodeType::Variable) {
		return new VariableLambda(lookup_symbol(ast->symbol_tree, *ast->data)->id);
	}
	else if (ast->type == ASTNodeType::Abstraction) {
		return new DeclarationLambda(ast->symbol_tree->id, run(ast->children[0]));
	}
	else if (ast->type == ASTNodeType::Application) {
		InterpreterLambda* func = run(ast->children[0]);
		InterpreterLambda* arg = run(ast->children[1]);
		
		InterpreterLambda* result = bind(func, arg, this);
		delete func;
		delete arg;
		return result;
	}
	else if (ast->type == ASTNodeType::Builtin) {
		std::string builtin_name = *ast->data;
		return new BuiltinLambda(nameToBuiltinType.at(builtin_name), {});
	}

	return nullptr;
}

int Interpreter::get_symbols_used(ASTNode* ast)
{
	int max = 0;
	if (ast->symbol_tree != nullptr) max = ast->symbol_tree->id;

	for (ASTNode* child : ast->children) {
		max = std::max(max, get_symbols_used(child));
	}

	return max;
}

int Interpreter::get_new_symbol()
{
	return symbols_used++;
}

Interpreter::Interpreter(ASTNode* ast, std::istream* input, std::ostream* output)
{
	this->ast = ast;
	this->input = input;
	this->output = output;
	symbols_used = get_symbols_used(ast);
}

void Interpreter::run()
{
	InterpreterLambda* result = run(ast);
	result->print(0, *output);
	delete result;
}