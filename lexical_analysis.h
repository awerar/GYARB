#pragma once

#include <vector>
#include <string>
#include "grammar_t.h"

using namespace std;

vector<string> parse_tokens(string program, grammar_t* grammar);