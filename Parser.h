#pragma once
#include "Lexer.h"
#include <stack>
#include "ParseNode.h"
#include <vector>
#include <map>
#include <any>

//Simple LR Parser

/*
<Grammar>
Goal: LambdaTerm Eof
LambdaTerm: LambdaTerm(LambdaTerm)
LambdaTerm: Id
LambdaTerm: lam Variable { LambdaTerm }
LambdaTerm: READC | PRINTC | READN | PRINTN
*/

typedef std::pair<int, ParseNode*> ParseStackFrame;
typedef std::stack<ParseStackFrame> ParseStack;

class Parser
{
	Lexer* lexer;
	ParserTable table;

public:
	Parser(Lexer* lexer);
	~Parser();
};