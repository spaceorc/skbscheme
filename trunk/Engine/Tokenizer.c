#include <assert.h>

#include "Tokenizer.h"

int IsEnd(LimitedStr * input) {
	return 0 == CurrentChr(input);
}

int IsOpeningBracket(LimitedStr * input) {
	return '(' == CurrentChr(input);
}

int IsClosingBracket(LimitedStr * input) {
	return ')' == CurrentChr(input);
}

int IsWhitespace(LimitedStr * input) {
	return CurrentChr(input) <= 32;
}

void SkipWhitespaces(LimitedStr * input) {
	while (!IsEnd(input) && IsWhitespace(input))
		IterateChr(input);
}

void SkipSymbol(LimitedStr * input) {
	while (!IsEnd(input) && !IsOpeningBracket(input) && !IsClosingBracket(input) && !IsWhitespace(input))
		IterateChr(input);
}

Token GetToken(LimitedStr * input) {
	Token result;
	result.tag = tokEnd;
	result.range.str = 0;
	result.range.size = 0;
	SkipWhitespaces(input);
	if (!IsEnd(input)) {
		result.range.str = input->str;
		if (IsOpeningBracket(input)) {
			result.tag = tokOpeningBracket;
			IterateChr(input);
		}
		else if (IsClosingBracket(input))	{
			result.tag = tokClosingBracket;
			IterateChr(input);
		}
		else {
			result.tag = tokSymbol;
			SkipSymbol(input);
		}
		result.range.size = input->str - result.range.str;
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
		case tokEnd:
			return "End";
		default:
			assert(0);
			return 0;
	}
}