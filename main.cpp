// GYARB.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "main.h"
#include "Parser.h"

using namespace std;

int main()
{
    initialize_grammar();

    Lexer* lexer = get_lexer("program.txt");
    Parser* parser = new Parser(lexer);
    ParseNode* result = parser->parse();
    std::cout << "FINAL TREE:\n";
    print_parse_tree(result);
}

Lexer* get_lexer(string file_name) {
    return new Lexer(new ifstream(file_name));
}