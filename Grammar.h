#pragma once

#include <vector>
#include "Token.h"

typedef std::pair<Token, std::vector<Token>> GrammarRule;
typedef std::vector<GrammarRule> Grammar;