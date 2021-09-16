#pragma once
#include <vector>
#include <any>
#include <map>
#include "Token.h"
#include "Parser.h"
#include "Grammar.h"

class ParserTable
{
	enum class ActionType
	{
		Shift, Reduce, Done, Error, Goto
	};

	typedef std::pair<ActionType, std::any> Action;

	std::vector<std::map<Token, Action>> actions;
	const Grammar grammar;

	Action get_action(int state, Token look_ahead) const;
	int get_goto(int state, Token non_terminal) const;

public:
	ParserTable(Grammar grammar);
	bool perform(ParseStack* stack, Lexer* lexer) const;
};