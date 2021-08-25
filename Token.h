#pragma once

enum class Token {
	Id, Eof,

	//Keywords
	Lambda,

	//Special Lambdas
	ReadNum, ReadChar, PrintNum, PrintChar,

	//Symbols
	OpenParentheses = '(', ClosedParebtheses = ')', OpenCurlyBracket = '{', ClosedCurlyBracket = '}'
};