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
	std::vector<std::pair<std::regex, std::string>> regex2terminal;

public:
	static grammar_t parse(std::string json_file_path);
	std::vector<std::string> parse_tokens(std::string program_file_path);

private:
	grammar_t(json json);
};

