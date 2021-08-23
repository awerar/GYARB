// GYARB.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "grammar_t.h"

int main()
{
    grammar_t g = grammar_t::parse("grammar.json");
}