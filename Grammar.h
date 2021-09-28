#pragma once

#include <vector>
#include "Token.h"

typedef std::pair<Token, std::vector<Token>> GrammarRule;
typedef std::vector<GrammarRule> Grammar;

const Grammar grammar = Grammar {
	GrammarRule(Token::Program, { Token::LambdaTerm }),
	GrammarRule(Token::LambdaTerm, { Token::LambdaTerm, Token::OpenParentheses, Token::LambdaTerm, Token::ClosedParebtheses }),
	GrammarRule(Token::LambdaTerm, { Token::Lambda, Token::Id, Token::OpenCurlyBracket, Token::LambdaTerm, Token::ClosedCurlyBracket }),
	GrammarRule(Token::LambdaTerm, { Token::Id }),
	GrammarRule(Token::LambdaTerm, { Token::ReadChar }),
	GrammarRule(Token::LambdaTerm, { Token::ReadNum }),
	GrammarRule(Token::LambdaTerm, { Token::PrintChar }),
	GrammarRule(Token::LambdaTerm, { Token::PrintNum }),
};