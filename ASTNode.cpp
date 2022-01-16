#include "ASTNode.h"
#include "Error.cpp"
#include <iostream>

ASTNode::ASTNode(ASTNodeType type, std::string* data, int start_size) : children(start_size)
{
    this->type = type;
    this->data = data;
}

ASTNode::~ASTNode()
{
    delete data;
    for (auto child : children) delete child;
    delete symbol_tree;
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

    if ((node.type == ASTNodeType::Abstraction || node.type == ASTNodeType::Variable) && node.symbol_tree != nullptr && node.data != nullptr) {
        out << " " << lookup_symbol(node.symbol_tree, *node.data)->id;
    }
    else if (node.data != nullptr) {
        out << " " << *node.data;
    }

    return out;
}

ASTNode* generate_ast(ParseNode* parse_tree)
{
    ASTNode* node = nullptr;

    switch (parse_tree->token)
    {
    case Token::LambdaTerm:
    case Token::Id:
        return generate_ast(parse_tree->children[0]);

    case Token::LambdaApplication:
        node = new ASTNode(ASTNodeType::Application, nullptr);

        for (ParseNode* child : parse_tree->children) {
            if (child->token == Token::LambdaTerm) node->children.push_back(generate_ast(child));
        }

        break;

    case Token::LambdaAbstraction:
        node = new ASTNode(ASTNodeType::Abstraction, nullptr, 1);

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

    if (node != nullptr) return node;
    throw RuntimeError{ "Error building AST"};
}

void print_AST(ASTNode* tree, int depth)
{
    for (int i = 0; i < depth; i++) std::cout << "   ";
    std::cout << *tree << std::endl;

    for (auto child : tree->children) print_AST(child, depth + 1);
}

void generate_symbol_tree(ASTNode* ast, SymbolNode* parent, std::shared_ptr<int> node_count)
{
    if (node_count == nullptr) node_count = std::make_shared<int>(0);

    SymbolNode* node = parent;
    if (ast->type == ASTNodeType::Abstraction) {
        SymbolNode* new_node = new SymbolNode();
        new_node->id = *node_count;
        new_node->symbol = *ast->data;
        new_node->parent = parent;
        if (parent != nullptr) parent->children.push_back(new_node);

        (*node_count)++;

        node = new_node;
    }

    ast->symbol_tree = node;

    for (auto child : ast->children) {
        generate_symbol_tree(child, node, node_count);
    }
}

SymbolNode* lookup_symbol(SymbolNode* node, std::string symbol)
{
    if (node == nullptr) return nullptr;
    else if (node->symbol == symbol) return node;
    else return lookup_symbol(node->parent, symbol);
}
