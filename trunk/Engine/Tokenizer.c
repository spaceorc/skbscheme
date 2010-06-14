#include <assert.h>

#include "Tokenizer.h"

int IsEnd(ConstStr * input) {
	if (*input == 0 || **input == 0)
		return 1;
	return 0;
}

int IsOpeningBracket(ConstStr * input) {
	if (**input == '(')
		return 1;
	return 0;
}

int IsClosingBracket(ConstStr * input) {
	if (**input == ')')
		return 1;
	return 0;
}

int IsWhitespace(ConstStr * input) {
	if (**input <= 32)
		return 1;
	return 0;
}

void SkipWhitespaces(ConstStr * input) {
	while (!IsEnd(input) && IsWhitespace(input)) {
		*input = *input + 1;
	}
}

void SkipSymbol(ConstStr * input) {
	while (!IsEnd(input) && !IsOpeningBracket(input) && !IsClosingBracket(input) && !IsWhitespace(input)) {
		*input = *input + 1;
	}
}

Token GetToken(ConstStr * input) {
	Token result;
	SkipWhitespaces(input);
	if (IsEnd(input)) {
		result.tag = tokEnd;
		*input = 0;
	}
	else if (IsOpeningBracket(input)) {
		result.tag = tokOpeningBracket;
		*input = *input + 1;
	}
	else if (IsClosingBracket(input))	{
		result.tag = tokClosingBracket;
		*input = *input + 1;
	}
	else {
		result.tag = tokSymbol;
		result.symbol.str = *input;
		SkipSymbol(input);
		result.symbol.size = *input - result.symbol.str;
	}
	return result;
}

const char * DumpTok(int tag) {
	switch(tag) {
		case tokOpeningBracket:
			return "OpeningBracket";
		case tokClosingBracket:
			return "ClosingBracket";
		case tokSymbol:
			return "Symbol";
		case tokError:
			return "Error";
		case tokEnd:
			return "End";
		default:
			assert(0);
	}
}