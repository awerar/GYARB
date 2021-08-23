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
	start = json["start"].get<std::string>();

	for (auto& nt : json["nonterminals"]) non_terminals.insert(nt.get<std::string>());
	for (auto& t : json["terminals"])
	{
		std::cout << json["terminals"][t.get<std::string>()];
	}

}