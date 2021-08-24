#include "lexical_analysis.h"

vector<string> parse_tokens(string program, grammar_t* grammar)
{
	vector<string> tokens;

	while(program.length() > 0) {
		int chunk_length = program.find(' ');
		if (chunk_length == string::npos) chunk_length = program.length();

		string::iterator chunk_end = program.begin() + chunk_length;

		string chunk(program.begin(), chunk_end);
		program = string(chunk_end + 2, program.end());

		if (chunk.length() == 0) continue;

		bool could_parse = false;
		for (auto parser : grammar->terminal_parsers) {
			if (regex_match(chunk, parser.second)) {
				tokens.push_back(chunk);

				could_parse = true;
				break;
			}
		}

		if (!could_parse) {
			throw;
		}
	}

	return tokens;
}
