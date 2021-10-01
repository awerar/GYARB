#include "ASTNode.h"
#include <iostream>

ASTNode::ASTNode(ASTNodeType type, std::string* data)
{
}

std::string ASTNode::print()
{
	switch (type)
	{
    case ASTNode::ASTNodeType::Variable:
        std::cout << "VARIABLE";
    case ASTNode::ASTNodeType::Definition:
        std::cout << "LAMBDA DEFINITION";
    case ASTNode::ASTNodeType::Application:
        std::cout << "APPLICATION";
    case ASTNode::ASTNodeType::Builtin:
        std::cout << "BUILTIN";
	}

    if (data != NULL) std::cout << "(" << *data << ")";
}
