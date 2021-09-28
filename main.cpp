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
    Lexer* lexer = get_lexer("program.txt");
    Parser* parser = new Parser(lexer);
}

Lexer* get_lexer(string file_name) {
    return new Lexer(new ifstream(file_name));
}