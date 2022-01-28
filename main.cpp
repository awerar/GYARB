// GYARB.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "main.h"
#include "Parser.h"
#include "ASTNode.h"
#include "SemanticAnalysis.h"
#include "Interpreter.h"

//Churchscript

using namespace std;

int main(int argc, char* argv[])
{
    if (argc <= 1) {
        std::cout << "You have to specifiy a churchscript file!"<<std::endl;
        return 1;
    }

    initialize_grammar();

    Lexer* lexer = get_lexer(argv[1]);
    Parser* parser = new Parser(lexer);
    ParseNode* cst = parser->parse();
    ASTNode* ast = generate_ast(cst);
    //print_AST(ast);

    generate_symbol_tree(ast);
    verify_semantics(ast);

    //print_AST(ast);

    Interpreter* interpreter = new Interpreter(ast, &std::cin, &std::cout);
    interpreter->run();

    return 0;
}

Lexer* get_lexer(string file_name) {
    return new Lexer(new ifstream(file_name));
}