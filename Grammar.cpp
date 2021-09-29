#include "Grammar.h"
#include <cassert>
#include <string>

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

std::string get_token_name(Token token)
{
	switch (token)
	{
	case Token::Id:
		return "id";
	case Token::Eof:
		return "eof";
	case Token::Lambda:
		return "lam";
	case Token::ReadNum:
		break;
	case Token::ReadChar:
		break;
	case Token::PrintNum:
		break;
	case Token::PrintChar:
		break;
	case Token::OpenParentheses:
		return "(";
	case Token::ClosedParebtheses:
		return ")";
	case Token::OpenCurlyBracket:
		return "{";
	case Token::ClosedCurlyBracket:
		return "}";
	case Token::Program:
		break;
	case Token::LambdaTerm:
		return "LAMBDA";
	default:
		break;
	}

	return std::to_string((int)token);
}
