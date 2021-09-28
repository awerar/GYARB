#include "Token.h"

bool is_terminal(Token token)
{
	return !is_non_terminal(token);
}

bool is_non_terminal(Token token)
{
	return non_terminals.find(token) != non_terminals.end();
}
