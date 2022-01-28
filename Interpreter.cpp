#include "Interpreter.h"
#include <iostream>
#include "Error.cpp"

Shared<InterpreterLambda> Interpreter::run(ASTNode* ast)
{
	if (ast->type == ASTNodeType::Variable) {
		return MakeShared<VariableLambda>(lookup_symbol(ast->symbol_tree, *ast->data)->id);
	}
	else if (ast->type == ASTNodeType::Abstraction) {
		return MakeShared<DeclarationLambda>(ast->symbol_tree->id, run(ast->children[0]));
	}
	else if (ast->type == ASTNodeType::Application) {
		Shared<InterpreterLambda> func = run(ast->children[0]);
		Shared<InterpreterLambda> arg = run(ast->children[1]);
		
		Shared<InterpreterLambda> result = bind(func, arg, this);
		return result;
	}
	else if (ast->type == ASTNodeType::Builtin) {
		std::string builtin_name = *ast->data;
		return MakeShared<BuiltinLambda>(nameToBuiltinType.at(builtin_name), std::vector<Shared<InterpreterLambda>>());
	}
	else if (ast->type == ASTNodeType::Number) {
		int num = std::stoi(*ast->data);
		return numToLambda(num, this);
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
	Shared<InterpreterLambda> result = run(ast);
	//result->print(0, *output);
}