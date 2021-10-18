#pragma once
#include <vector>
#include <string>
#include "ParserStack.h"

enum class ASTNodeType {
	Variable, Abstraction, Application, Builtin
};

struct ASTNode
{
	ASTNodeType type;
	std::string* data;
	std::vector<ASTNode*> children;

	ASTNode(ASTNodeType type, std::string* data, int start_size = 0);
	friend std::ostream& operator<<(std::ostream& out, ASTNode const& node);
};

void print_AST(ASTNode* tree, int depth = 0);
ASTNode* generate_ast(ParseNode* parse_tree);