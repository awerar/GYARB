#pragma once

#include <vector>
#include <unordered_set>

enum class Token {
	Id, Eof,

	//Keywords
	Lambda,

	//Special Lambdas
	ReadNum, ReadChar, PrintNum, PrintChar,

	//Symbols
	OpenParentheses = '(', ClosedParebtheses = ')', OpenCurlyBracket = '{', ClosedCurlyBracket = '}',

	//Non terminals
	Goal, LambdaTerm, 
};

std::unordered_set<Token> non_terminals = {
	Token::Goal, Token::LambdaTerm
};

bool is_terminal(Token token);
bool is_non_terminal(Token token);