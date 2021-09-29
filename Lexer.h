#pragma once

#include "Grammar.h"
#include <string>

class Lexer {
public:
	explicit Lexer(std::istream& is);
	explicit Lexer(std::istream* ps);

	// A lexer belongs to a parser and should not be copied or moved.

	Lexer(const Lexer&) = delete;
	Lexer& operator=(const Lexer&) = delete;

	Lexer(Lexer&&) = delete;
	Lexer& operator=(Lexer&&) = delete;

	~Lexer() { if (owns_input) delete p_input; }

	Token token() const { return cur_token; }
	std::string text() const { return token_buffer; }

	void advance() { cur_token = get_token(); }

private:
	std::istream* p_input;	// The source stream (a stream of characters).
	bool owns_input;		// True if we can delete p_input, false if we can't.

	Token cur_token;
	std::string token_buffer;

	Token get_token();		// The workhorse. Assembles characters from p_input into tokens.
};