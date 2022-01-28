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
	OpenParentheses = '(', ClosedParentheses = ')', OpenCurlyBracket = '{', ClosedCurlyBracket = '}', Colon = ':', Number,

	//Non terminals
	Start, LambdaTerm, LambdaAbstraction, LambdaApplication, Id
};

typedef std::pair<Token, std::vector<Token>> GrammarRule;
typedef std::vector<GrammarRule> Grammar;

const int max_rule_size = 10;
const Grammar grammar = Grammar {
	GrammarRule(Token::Start, { Token::LambdaTerm, Token::Eof }),
	GrammarRule(Token::LambdaTerm, { Token::LambdaApplication }),
	GrammarRule(Token::LambdaApplication, { Token::LambdaTerm, Token::OpenParentheses, Token::LambdaTerm, Token::ClosedParentheses }),
	GrammarRule(Token::LambdaTerm, { Token::LambdaAbstraction }),
	GrammarRule(Token::LambdaAbstraction, { Token::Lambda, Token::Variable, Token::OpenCurlyBracket, Token::LambdaTerm, Token::ClosedCurlyBracket }),
	GrammarRule(Token::LambdaAbstraction, { Token::Lambda, Token::Variable, Token::Colon, Token::LambdaTerm }),
	GrammarRule(Token::LambdaTerm, { Token::Id }),
	GrammarRule(Token::Id, { Token::Builtin }),
	GrammarRule(Token::Id, { Token::Variable }),
	GrammarRule(Token::LambdaTerm, { Token::Number }),
};

extern std::unordered_map<Token, std::unordered_set<int>> token2rules;
extern std::unordered_set<Token> terminals, non_terminals, tokens;

void initialize_grammar();
bool is_non_terminal(Token token);
bool is_terminal(Token token);
std::string get_token_name(Token token);

/*#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "Grammar.h"

enum class Token {
	Eof, Variable, Builtin, Let,

	//Keywords
	Lambda,

	//Symbols
	OpenParentheses = '(', ClosedParentheses = ')', OpenCurlyBracket = '{', 
	ClosedCurlyBracket = '}', Colon = ':', Number, Equals = '=',

	//Non terminals
	Start, LambdaTerm, LambdaAbstraction, LambdaApplication, Id, MainProgram, Definition
};

typedef std::pair<Token, std::vector<Token>> GrammarRule;
typedef std::vector<GrammarRule> Grammar;

const int max_rule_size = 10;
const Grammar grammar = Grammar {
	GrammarRule(Token::Start, { Token::MainProgram, Token::Eof }),
	GrammarRule(Token::LambdaTerm, { Token::LambdaApplication }),
	GrammarRule(Token::LambdaApplication, { Token::LambdaTerm, Token::OpenParentheses, Token::LambdaTerm, Token::ClosedParentheses }),
	GrammarRule(Token::LambdaTerm, { Token::LambdaAbstraction }),
	GrammarRule(Token::LambdaAbstraction, { Token::Lambda, Token::Variable, Token::OpenCurlyBracket, Token::LambdaTerm, Token::ClosedCurlyBracket }),
	GrammarRule(Token::LambdaAbstraction, { Token::Lambda, Token::Variable, Token::Colon, Token::LambdaTerm }),
	GrammarRule(Token::LambdaTerm, { Token::Id }),
	GrammarRule(Token::Id, { Token::Builtin }),
	GrammarRule(Token::Id, { Token::Variable }),
	GrammarRule(Token::LambdaTerm, { Token::Number }),
};

extern std::unordered_map<Token, std::unordered_set<int>> token2rules;
extern std::unordered_set<Token> terminals, non_terminals, tokens;

void initialize_grammar();
bool is_non_terminal(Token token);
bool is_terminal(Token token);
std::string get_token_name(Token token);*/