#include "Grammar.h"
#include <cassert>

std::unordered_map<Token, std::unordered_set<int>> token2rules;
std::unordered_set<Token> terminals, non_terminals, tokens;

void initialize_grammar()
{
	for (int i = 0; i < grammar.size(); i++)
	{
		auto& [from, to] = grammar[i];
		assert(to.size() <= max_rule_size);

		token2rules[from].insert(i);

		for (Token t : to) {
			tokens.insert(t);
		}

		non_terminals.insert(from);
	}

	terminals = tokens;
	for (Token nt : non_terminals) terminals.erase(nt);
}

bool is_non_terminal(Token token)
{
	return non_terminals.count(token) > 0;
}

bool is_terminal(Token token)
{
	return terminals.count(token) > 0;
}
