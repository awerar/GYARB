#include "Lexer.h"
#include <istream>
#include "Error.cpp"

Lexer::Lexer(std::istream& is) : p_input{ &is }, owns_input{ false }
{
	advance();
}

Lexer::Lexer(std::istream* ps) : p_input{ ps }, owns_input{ true }
{
	advance();
}

Token Lexer::get_token()
{
	std::istream& input = *p_input;	// shorthand to make the notation convenient.

	token_buffer.clear();			// clear the buffer for the new token.

	char c = input.get();			// a priming read on the stream.

	// Skip whitespace.
	while (isspace(c)) c = input.get();

	// If there are no characters, we're at the end of the stream.
	if (!input) return Token::Eof;

    if (isalpha(c)) {
        token_buffer = c;
        c = input.get();

        // Look for zero or more letters or digits.
        while (isalnum(c)) {
            token_buffer += c;
            c = input.get();
        }

        // The current character doesn't belong to our identifier, and must be put back into the stream.
        input.putback(c);

        // Check for a special name.
        if (token_buffer == "lam") return Token::Lambda;
        if (token_buffer == "PRINTN") return Token::PrintNum;
        if (token_buffer == "PRINTC") return Token::PrintChar;
        if (token_buffer == "READN") return Token::ReadNum;
        if (token_buffer == "READC") return Token::ReadChar;

        // Whatever is not a function name must be an identifier.
        return Token::Id;
    }

    // Check for a single character token.
    token_buffer = c;
    switch (c) {
        // Note: fallthrough intentional.
        case '(':
        case ')':
        case '{':
        case '}':
        return Token(c);
    }

    throw Lexical_error{ token_buffer };
}