#pragma once
#include <vector>
#include <string>
#include "ParserStack.h"
#include "SymbolTree.h"

enum class ASTNodeType {
	Variable, Abstraction, Application, Builtin
};

struct ASTNode
{
	ASTNodeType type;
	std::string* data;
	std::vector<ASTNode*> children;
	SymbolNode* symbol_tree = NULL;

	ASTNode(ASTNodeType type, std::string* data, int start_size = 0);
	~ASTNode();
	friend std::ostream& operator<<(std::ostream& out, ASTNode const& node);
};

void print_AST(ASTNode* tree, int depth = 0);
ASTNode* generate_ast(ParseNode* parse_tree);

void generate_symbol_tree(ASTNode* ast, SymbolNode* parent = NULL, std::shared_ptr<int> node_count = nullptr);
SymbolNode* lookup_symbol(SymbolNode* node, std::string symbol);