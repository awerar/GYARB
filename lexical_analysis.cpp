#include "lexical_analysis.h"

vector<string> parse_tokens(string program, grammar_t* grammar)
{
	vector<string> tokens;

	while (program.length() > 0) {
		bool has_parsed = false;

		for (auto& parser : grammar->terminal_parsers) {
			
		}

		assert(has_parsed);
	}
}
