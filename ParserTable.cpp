#include "ParserTable.h"
#include "Error.cpp"
#include "Grammar.h"

#include <unordered_map>

ParserTable::Action ParserTable::get_action(int state, Token look_ahead) const
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
			return actions[state].at(look_ahead);
		}
	}
}

int ParserTable::get_goto(int state, Token non_terminal) const
{
	Action action = get_action(state, non_terminal);

	if (action.first != ActionType::Goto)
	{
		throw Syntax_error{ "Invalid action type: not Goto" };
	}

	return std::any_cast<int>(action.first);
}

ParserTable::ParserTable(Grammar grammar) : grammar(grammar)
{
	for (const auto& [from, to] : grammar)
	{
		for (Token t : to) tokens.insert(t);
		tokens.insert(from);
	}

	for (Token t : tokens) follows[t] = { Token::Eof };

	for (const auto& [from, to] : grammar)
	{
		for (auto t_iter = to.begin(); (t_iter + 1) != to.end(); t_iter++)
		{
			follows[*t_iter].insert(*(t_iter + 1));
		}
	}
}

bool ParserTable::perform(ParseStack* stack, Lexer* lexer) const
{
	int curr_state = stack->top().first;
	Token look_ahead = lexer->token();

	Action action = get_action(curr_state, look_ahead);
	auto& [type, data] = action;

	if (type == ActionType::Shift)
	{
		int next_state = std::any_cast<int>(data);
		ParseStackFrame new_stack_frame(next_state, new ParseNode(look_ahead));

		stack->push(new_stack_frame);
		lexer->advance();
	}
	else if (type == ActionType::Reduce)
	{
		GrammarRule rule = grammar.at(std::any_cast<int>(data));
		const Token rhs = rule.first;
		int rhs_size = rule.second.size();

		ParseNode* parent = new ParseNode(rule.first);
		for (int i = 0; i < rhs_size; i++)
		{
			ParseNode* child = stack->top().second;
			stack->pop();

			parent->children.push_back(child);
		}

		int old_state = stack->top().first;
		int new_state = get_goto(old_state, rhs);
		stack->push(ParseStackFrame(new_state, parent));
	}
	else if (type == ActionType::Done) return true;
	else if (type == ActionType::Error) throw Syntax_error{ "Error while parsomg with following error message: " + std::any_cast<std::string>(data) };

	return false;
}