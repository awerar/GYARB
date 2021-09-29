#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Grammar.h"

enum class Token {
	Id, Eof,

	//Keywords
	Lambda,

	//Special Lambdas
	ReadNum, ReadChar, PrintNum, PrintChar,

	//Symbols
	OpenParentheses = '(', ClosedParebtheses = ')', OpenCurlyBracket = '{', ClosedCurlyBracket = '}',

	//Non terminals
	Program, LambdaTerm,
};

typedef std::pair<Token, std::vector<Token>> GrammarRule;
typedef std::vector<GrammarRule> Grammar;


const int max_rule_size = 10;
const Grammar grammar = Grammar {
	GrammarRule(Token::Program, { Token::LambdaTerm, Token::Eof }),
	GrammarRule(Token::LambdaTerm, { Token::LambdaTerm, Token::OpenParentheses, Token::LambdaTerm, Token::ClosedParebtheses }),
	GrammarRule(Token::LambdaTerm, { Token::Lambda, Token::Id, Token::OpenCurlyBracket, Token::LambdaTerm, Token::ClosedCurlyBracket }),
	GrammarRule(Token::LambdaTerm, { Token::Id }),
	GrammarRule(Token::LambdaTerm, { Token::ReadChar }),
	GrammarRule(Token::LambdaTerm, { Token::ReadNum }),
	GrammarRule(Token::LambdaTerm, { Token::PrintChar }),
	GrammarRule(Token::LambdaTerm, { Token::PrintNum }),
};

/*const Grammar grammar = Grammar{
	GrammarRule(Token::Program, { Token::LambdaTerm, Token::Eof }),
	GrammarRule(Token::LambdaTerm, { Token::Id, Token::LambdaTerm,  }),
	GrammarRule(Token::LambdaTerm, { Token::Id }),
};*/

extern std::unordered_map<Token, std::unordered_set<int>> token2rules;
extern std::unordered_set<Token> terminals, non_terminals, tokens;

void initialize_grammar();
bool is_non_terminal(Token token);
bool is_terminal(Token token);