#include "ParserTable.h"
#include "Error.cpp"
#include "Grammar.h"
#include <memory>

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

	return std::any_cast<int>(action.second);
}

ParserTable::ParserTable()
{
	compute_follows();

	ParseItemSet first_set = { { 0, 0 } };

	auto sets = new std::vector<ParseItemSet>();
	insert_item_set(first_set, sets);
	add_reductions(sets);

	delete sets;
}

void ParserTable::add_reductions(std::vector<ParseItemSet>* sets) {
	for (int set_id = 0; set_id < sets->size(); set_id++) {
		for (auto& [rule_id, dot] : sets->at(set_id)) {
			if (dot >= grammar[rule_id].second.size()) {
				for (Token t : tokens) {
					if (must_have_reduction(t, rule_id)) {
						actions[set_id][t] = Action(ActionType::Reduce, rule_id);
					}
				}
			}
		}
	}
}

bool ParserTable::must_have_reduction(Token token, int rule_id) {
	return follows[grammar[rule_id].first].count(token) >= 1;
}

int ParserTable::insert_item_set(ParserTable::ParseItemSet new_set, std::vector<ParserTable::ParseItemSet>* sets) {
	new_set = add_closure(new_set);

	for (int i = 0; i < sets->size(); i++) {
		auto& set = sets->at(i);

		if (items_sets_identical(set, new_set)) return i;
	}

	actions.push_back({ });
	sets->push_back(new_set);
	int new_set_id = sets->size() - 1;

	std::unordered_map<Token, ParseItemSet> next_sets;

	for (ParseItem item : new_set) {
		auto& [rule_id, dot] = item;
		const auto& rule = grammar[rule_id];

		if (dot < rule.second.size()) {
			if (rule.second[dot] == Token::Eof) {
				actions[new_set_id][rule.second[dot]] = Action(ActionType::Done, NULL);
			}
			else {
				ParseItem core_item = { rule_id, dot + 1 };
				next_sets[rule.second[dot]].insert(core_item);
			}
		}
	}

	for (auto next_set : next_sets) {
		int transition_state = insert_item_set(next_set.second, sets);
		actions[new_set_id][next_set.first] = Action(is_terminal(next_set.first) ? ActionType::Shift : ActionType::Goto, transition_state);
	}

	return new_set_id;
}

ParserTable::ParseItemSet ParserTable::add_closure(ParserTable::ParseItemSet set) const
{
	ParseItemSet result = set;
	ParseItemSet prev_items = set;
	do
	{
		ParseItemSet new_items;

		for (auto& [rule_id, dot] : prev_items)
		{
			auto& rule = grammar[rule_id];

			if (dot >= rule.second.size()) continue;

			Token following = rule.second[dot];
			if (is_non_terminal(following))
			{
				for (auto closure_rule_id : token2rules[following])
				{
					ParseItem new_item = { closure_rule_id, 0 };
					if (result.find(new_item) == result.end()) new_items.insert(new_item);
				}
			}
		}

		prev_items = new_items;
		result.insert(new_items.begin(), new_items.end());
	} while (prev_items.size() > 0);

	return result;
}

void ParserTable::compute_follows() {
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
		parent->children.resize(rhs_size);
		for (int i = 0; i < rhs_size; i++)
		{
			ParseNode* child = stack->top().second;
			stack->pop();

			parent->children.at(rhs_size - i - 1) = child;
		}

		int old_state = stack->top().first;
		int new_state = get_goto(old_state, rhs);
		stack->push(ParseStackFrame(new_state, parent));
	}
	else if (type == ActionType::Done) return true;
	else if (type == ActionType::Error) throw Syntax_error{ "Error while parsomg with following error message: " + std::any_cast<std::string>(data) };

	return false;
}

size_t ParserTable::ParseItemHash::operator()(const ParseItem& parse_item) const
{
	auto& [rule, dot] = parse_item;
	return (max_rule_size + 1) * rule + dot;
}

bool ParserTable::items_sets_identical(const ParserTable::ParseItemSet& a, const ParserTable::ParseItemSet& b) {
	if (a.size() != b.size()) return false;

	for (ParseItem pi : a) {
		if (b.count(pi) == 0) return false;
	}

	return true;
}

std::ostream& operator<<(std::ostream& out, ParserTable const& table) {
	std::cout << "   ";

	for (Token t : tokens) {
		std::string msg = get_token_name(t);
		std::cout << msg;

		for (int i = 4; i > msg.size(); i--) out << " ";
	}

	std::cout << "\n";

	for (int i = 0; i < table.actions.size(); i++) {
		const auto& action = table.actions[i];

		out << i;
		if (std::to_string(i).size() == 1) out << " ";

		for (Token t : tokens) {
			std::cout << "|";

			std::string msg;

			if (action.count(t) == 0) msg = "";
			else {
				switch (action.at(t).first)
				{
				default:
					msg = "?";
					break;

				case ParserTable::ActionType::Shift:
					msg = "s" + std::to_string(std::any_cast<int>(action.at(t).second));
					break;

				case ParserTable::ActionType::Goto:
					msg = std::to_string(std::any_cast<int>(action.at(t).second));
					break;

				case ParserTable::ActionType::Reduce:
					msg = "r" + std::to_string(std::any_cast<int>(action.at(t).second));
					break;

				case ParserTable::ActionType::Done:
					msg = "acc";
					break;
				}
			}

			out << msg;

			for (int i = 3; i > msg.size(); i--) out << " ";
		}

		out << "\n";
	}

	return out;
}