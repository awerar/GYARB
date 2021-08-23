#include "grammar_t.h"
#include <fstream>
#include <iostream>

grammar_t grammar_t::parse(std::string json_file_path)
{
	std::ifstream json_file(json_file_path);
	std::string json_content(++++++(std::istreambuf_iterator<char>(json_file)),
							  std::istreambuf_iterator<char>());
	std::cout << json_content << std::endl;
	json_file.close();

	json json = json::parse(json_content);
	return grammar_t(json);
}

grammar_t::grammar_t(json json)
{	
	std::vector<std::string> nt = json["nonterminals"];
	terminals.insert(nt.begin(), nt.end());

	for (auto& [t, regx] : json["terminals"].items()) {
		terminals.insert(t);
		regex2terminal.push_back(std::pair(std::regex(regx), t));
	}

	start = json["start"].get<std::string>();
	assert(non_terminals.find(start) != non_terminals.end());
}