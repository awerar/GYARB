#pragma once
#include "Lexer.h"
#include <stack>
#include <vector>
#include <map>
#include <any>
#include "ParserStack.h"
#include "ParserTable.h"

//Simple LR Parser

/*
<Grammar>
Goal: LambdaTerm Eof
LambdaTerm: LambdaTerm(LambdaTerm)
LambdaTerm: Id
LambdaTerm: lam Variable { LambdaTerm }
LambdaTerm: READC | PRINTC | READN | PRINTN
*/

class Parser
{
	Lexer* lexer;
	const ParserTable table;

public:
	Parser(Lexer* lexer);
	~Parser();
};