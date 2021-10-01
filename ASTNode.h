#pragma once
#include <vector>
#include <string>

struct ASTNode
{
	enum class ASTNodeType {
		Variable, Definition, Application, Builtin
	};

	ASTNodeType type;
	std::string* data;
	std::vector<ASTNode*> children;

	ASTNode(ASTNodeType type, std::string* data);
	std::string print();
};

