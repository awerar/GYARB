// GYARB.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "main.h"
#include "Parser.h"
#include "ASTNode.h"

using namespace std;

int main()
{
    initialize_grammar();

    Lexer* lexer = get_lexer("program.txt");
    Parser* parser = new Parser(lexer);
    ParseNode* cst = parser->parse();

    std::cout << "CST:\n";
    print_parse_tree(cst);
    std::cout << std::endl;

    ASTNode* ast = generate_ast(cst);

    std::cout << "AST:\n";
    print_AST(ast);
}

Lexer* get_lexer(string file_name) {
    return new Lexer(new ifstream(file_name));
}