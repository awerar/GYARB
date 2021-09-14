#pragma once
#include <vector>
#include <any>
#include <map>
#include "Token.h"
#include "Parser.h"

class ParserTable
{
	enum class ActionType
	{
		Shift, Reduce, Done, Error
	};

	typedef std::pair<ActionType, std::any> Action;

	std::vector<std::map<Token, Action>> actions;

	Action get_action(int state, Token look_ahead);

public:
	ParserTable(int states);
	void perform(int& state, ParseStack& stack, Token look_ahead) const;
	void add_state(int state, Token token, Action action);
};