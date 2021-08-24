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
	auto& nt = json["nonterminals"];
	non_terminals.insert(nt.begin(), nt.end());
	
	for (std::string terminal : json["terminals"]) {
		terminals.insert(terminal);

		auto& t2r = json["terminal2regex"];
		auto iter = t2r.find(terminal);
		std::string regex;
		if(iter != t2r.end()) regex = iter.value().get<std::string>();
		else {
			//Turns the terminal into a plaintext regex

			regex = terminal;
			for (int i = 0; i < terminal.length(); i++) {
				regex.insert(i * 2, "\\");
			}
		}

		terminal_parsers.push_back(std::pair(terminal, std::regex(regex)));
	}

	start = json["start"].get<std::string>();
	assert(non_terminals.find(start) != non_terminals.end());
}

