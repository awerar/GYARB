// GYARB.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "main.h"
#include "Parser.h"
#include "ASTNode.h"
#include "SemanticAnalysis.h"

using namespace std;

int main()
{
    initialize_grammar();

    Lexer* lexer = get_lexer("SimpleProgram.txt");
    Parser* parser = new Parser(lexer);
    ParseNode* cst = parser->parse();
    ASTNode* ast = generate_ast(cst);
    print_AST(ast);

    generate_symbol_tree(ast);
    verify_semantics(ast);
}

Lexer* get_lexer(string file_name) {
    return new Lexer(new ifstream(file_name));
}

//Lambda in memory:
/*
int type

0: Var
    Lambda* var_mem

1: Call
    Lambda* func
    Lambda* param

2: Definition
    Lambda* content
    Lambda* var

3: Builtin
    int id
    
*/

/*
%0 = [0, NULL]

%1 = reserve(3);
%2 = [2, %0, %1]
%0[1] = %1

%3 = reserve(3)
%4 = [2, %2, %3]

%5 = reserve(3)
%6 = [1, %5, %4]

%7 = [2, %6, %5]

%8 = [3, 0]
%9 = [1, %7, %8]
*/