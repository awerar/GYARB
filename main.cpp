// GYARB.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "main.h"

using namespace std;

int main()
{
    Lexer* lexer = get_lexer("program.txt");
    while (lexer->token() != Token::Eof) {
        auto token = lexer->token();
        lexer->advance();
    }
}

Lexer* get_lexer(string file_name) {
    return new Lexer(new ifstream(file_name));
}