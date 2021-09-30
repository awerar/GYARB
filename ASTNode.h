#pragma once
#include <vector>

struct ASTNode
{
	enum class ASTNodeType {
		Variable, Implementation, 
	};

	std::vector<ASTNode*> children;
};

