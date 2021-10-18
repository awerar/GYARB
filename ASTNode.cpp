#include "ASTNode.h"
#include "Error.cpp"
#include <iostream>

ASTNode::ASTNode(ASTNodeType type, std::string* data, int start_size) : children(start_size)
{
    this->type = type;
    this->data = data;
}

std::ostream& operator<<(std::ostream& out, ASTNode const& node)
{
    switch (node.type)
    {
    case ASTNodeType::Variable:
        out << "var";
        break;
    case ASTNodeType::Abstraction:
        out << "lam";
        break;
    case ASTNodeType::Application:
        out << "call";
        break;
    case ASTNodeType::Builtin:
        out << "builtin";
        break;
    }

    if (node.data != NULL) out << " " << *node.data;

    return out;
}

ASTNode* generate_ast(ParseNode* parse_tree)
{
    ASTNode* node = NULL;

    switch (parse_tree->token)
    {
    case Token::LambdaTerm:
    case Token::Id:
        return generate_ast(parse_tree->children[0]);

    case Token::LambdaApplication:
        node = new ASTNode(ASTNodeType::Application, NULL);

        for (ParseNode* child : parse_tree->children) {
            if (child->token == Token::LambdaTerm) node->children.push_back(generate_ast(child));
        }

        break;

    case Token::LambdaAbstraction:
        node = new ASTNode(ASTNodeType::Abstraction, NULL, 1);

        for (ParseNode* child : parse_tree->children) {
            if (child->token == Token::Variable) node->data = new std::string(child->text);
            if (child->token == Token::LambdaTerm) node->children[0] = generate_ast(child);
        }

        break;

    case Token::Variable:
        return new ASTNode(ASTNodeType::Variable, new std::string(parse_tree->text));

    case Token::Builtin:
        return new ASTNode(ASTNodeType::Builtin, new std::string(parse_tree->text));
    }

    if (node != NULL) return node;
    throw RuntimeError{ "Error building AST"};
}

void print_AST(ASTNode* tree, int depth)
{
    for (int i = 0; i < depth; i++) std::cout << "   ";
    std::cout << *tree << std::endl;

    for (auto child : tree->children) print_AST(child, depth + 1);
}
