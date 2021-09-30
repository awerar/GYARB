#pragma once
#include <vector>
#include <any>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include "Grammar.h"
#include "ParserStack.h"
#include "Lexer.h"

class ParserTable
{
	enum class ActionType
	{
		Shift, Reduce, Done, Error, Goto
	};

	typedef std::pair<int, int> ParseItem;
	struct ParseItemHash { size_t operator()(const ParseItem& parse_item) const; };
	typedef std::unordered_set<ParseItem, ParseItemHash> ParseItemSet;

	typedef std::pair<ActionType, std::any> Action;

	friend std::ostream& operator<<(std::ostream& out, ParserTable const& v);
	friend std::ostream& operator<<(std::ostream& out, ParseItemSet const& item_set);

	std::vector<std::map<Token, Action>> actions;
	std::unordered_map<Token, std::unordered_set<Token>> follows;
	std::vector<ParseItemSet> kernel_sets;

	ParserTable::Action get_action(int state, Token look_ahead) const;
	int get_goto(int state, Token non_terminal) const;
	ParseItemSet add_closure(ParseItemSet set) const;
	void compute_follows();
	int insert_item_set(ParseItemSet item_set, std::vector<ParserTable::ParseItemSet>* sets);
	bool items_sets_identical(const ParserTable::ParseItemSet& a, const ParserTable::ParseItemSet& b);
	void add_reductions(std::vector<ParseItemSet>* sets);
	bool must_have_reduction(Token token, int rule_id);

public:
	ParserTable();
	bool perform(ParseStack* stack, Lexer* lexer) const;
};

std::ostream& operator<<(std::ostream& out, ParserTable::ParseItemSet const& item_set);