#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "Grammar.h"

enum class Token {
	Eof, Variable, Builtin,

	//Keywords
	Lambda,

	//Symbols
	OpenParentheses = '(', ClosedParentheses = ')', OpenCurlyBracket = '{', ClosedCurlyBracket = '}',

	//Non terminals
	Program, LambdaTerm, Id
};

typedef std::pair<Token, std::vector<Token>> GrammarRule;
typedef std::vector<GrammarRule> Grammar;

const int max_rule_size = 10;
const Grammar grammar = Grammar {
	GrammarRule(Token::Program, { Token::LambdaTerm, Token::Eof }),
	GrammarRule(Token::LambdaTerm, { Token::LambdaTerm, Token::OpenParentheses, Token::LambdaTerm, Token::ClosedParentheses }),
	GrammarRule(Token::LambdaTerm, { Token::Lambda, Token::Variable, Token::OpenCurlyBracket, Token::LambdaTerm, Token::ClosedCurlyBracket }),
	GrammarRule(Token::LambdaTerm, { Token::Id }),
	GrammarRule(Token::Id, { Token::Builtin }),
	GrammarRule(Token::Id, { Token::Variable }),
};

extern std::unordered_map<Token, std::unordered_set<int>> token2rules;
extern std::unordered_set<Token> terminals, non_terminals, tokens;

void initialize_grammar();
bool is_non_terminal(Token token);
bool is_terminal(Token token);
std::string get_token_name(Token token);