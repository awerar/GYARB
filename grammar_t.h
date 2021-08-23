#include <string>
#include <unordered_set>
#include <unordered_map>
#include <regex>
#include <vector>
#include <map>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#pragma once
class grammar_t
{
	std::string start;
	std::unordered_set<std::string> non_terminals, terminals;
	std::map<std::regex, std::string> regex2terminal;

public:
	static grammar_t parse(std::string json_file);

private:
	grammar_t(json json);
};

