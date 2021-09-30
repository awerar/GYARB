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
			return Action(ActionType::Error, "No action exists for token " + get_token_name(look_ahead) + " in state " + std::to_string(state));
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
		throw SyntaxError{ "Invalid action type: not Goto" };
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

int ParserTable::insert_item_set(ParserTable::ParseItemSet kernel_set, std::vector<ParserTable::ParseItemSet>* sets) {
	auto new_set = add_closure(kernel_set);

	for (int i = 0; i < sets->size(); i++) {
		auto& set = sets->at(i);

		if (items_sets_identical(set, new_set)) return i;
	}

	actions.push_back({ });
	sets->push_back(new_set);
	kernel_sets.push_back(kernel_set);
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

	for (const auto &next_set : next_sets) {
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

std::unordered_set<Token> get_beginning_of(Token token, std::shared_ptr<std::unordered_set<Token>> visited) {
	if (is_terminal(token)) return { token };

	if (visited->count(token) != 0) return {};
	visited->insert(token);

	std::unordered_set<Token> res;

	for (auto rule : token2rules[token]) {
		Token b = *grammar[rule].second.begin();

		if (is_terminal(b)) res.insert(b);
		else {
			auto starts = get_beginning_of(b, visited);
			res.insert(starts.begin(), starts.end());
		}
	}

	return res;
}

std::unordered_set<Token> get_ends_of(Token non_terminal, std::shared_ptr<std::unordered_set<Token>> visited)
{
	if (is_terminal(non_terminal)) return { };

	if (visited->count(non_terminal) != 0) return {};
	visited->insert(non_terminal);

	std::unordered_set<Token> res;

	for (auto rule : token2rules[non_terminal]) {
		Token e = grammar[rule].second.back();

		res.insert(e);
		auto ends = get_ends_of(e, visited);
		res.insert(ends.begin(), ends.end());
	}

	return res;
}

void ParserTable::compute_follows() {
	for (int i = 0; i < grammar.size(); i++)
	{
		const auto& [from, to] = grammar[i];

		for (auto t_iter = to.begin(); (t_iter + 1) != to.end(); t_iter++)
		{
			Token token = *t_iter;
			Token follow = *(t_iter + 1);

			//if (!is_non_terminal(token)) continue;

			auto token_ends = get_ends_of(token, std::make_shared<std::unordered_set<Token>>());
			token_ends.insert(token);

			auto follow_begins = get_beginning_of(follow, std::make_shared<std::unordered_set<Token>>());
			for (auto end : follow_begins) {
				for (auto beg : token_ends) {
					follows[beg].insert(end);
				}
			}
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
		ParseStackFrame new_stack_frame(next_state, new ParseNode(look_ahead, lexer->text()));

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
	else if (type == ActionType::Error) throw SyntaxError{ "Error while parsomg with following error message: " + std::any_cast<std::string>(data) };

	//std::cout << "<############ STACK ############>\n";
	//print_parse_stack(*stack);

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
	std::cout << "RULES:" << std::endl;

	for (int i = 0; i < grammar.size(); i++) {
		const auto& rule = grammar[i];

		out << i << ": " << get_token_name(rule.first) << " => ";

		for (Token t : rule.second) {
			out << get_token_name(t) << " ";
		}

		out << std::endl;
	}

	std::cout << std::endl << "   ";

	for (Token t : tokens) {
		std::string msg = get_token_name(t);
		msg.resize(3, ' ');
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

		std::cout << table.kernel_sets[i];

		out << "\n";
	}

	return out;
}

std::ostream& operator<<(std::ostream& out, ParserTable::ParseItemSet const& item_set)
{
	out << "[";
	for (const auto& item : item_set) {
		const auto& rule = grammar[item.first];
		out << get_token_name(rule.first) << " => ";
		for (int i = 0; i < rule.second.size(); i++) {
			if (item.second == i) out << "* ";
			out << get_token_name(rule.second[i]);

			if (i != rule.second.size() - 1) std::cout << " ";
		}

		if (item.second == rule.second.size()) std::cout << " *";

		if (item != *(--item_set.end())) out << ", ";
	}

	out << "]";
	return out;
}
