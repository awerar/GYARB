#include "SemanticAnalysis.h"
#include "Error.cpp"

void verify_symbols(ASTNode* ast)
{
	if (ast->type == ASTNodeType::Variable) {
		std::string symbol = *ast->data;
		SymbolNode* symbol_node = lookup_symbol(ast->symbol_tree, *ast->data);

		if (symbol_node == nullptr) {
			throw SyntaxError{ "Unknown symbol " + symbol };
		}
	}

	for (auto child : ast->children) verify_symbols(child);
}

void verify_semantics(ASTNode* ast)
{
	verify_symbols(ast);
}