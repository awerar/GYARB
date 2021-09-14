#include "ParserTable.h"
#include "Error.cpp"

ParserTable::Action ParserTable::get_action(int state, Token look_ahead)
{
	if (state >= this->actions.size())
	{
		return Action(ActionType::Error, "State " + std::to_string(state) + " is illegal");
	}
	else
	{
		auto& row = this->actions[state];
		if (row.count(look_ahead) == 0)
		{
			return Action(ActionType::Error, "No action exists for token in state " + std::to_string(state));
		}
		else
		{
			return actions[state][look_ahead];
		}
	}
}

ParserTable::ParserTable(int states)
{
	this->actions.resize(states);
}

void ParserTable::perform(int& state, ParseStack& stack, Token look_ahead) const
{

}

void ParserTable::add_state(int state, Token token, Action action)
{
	this->actions[state][token] = action;
}
