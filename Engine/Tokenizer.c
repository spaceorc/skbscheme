#include <assert.h>

#include "Tokenizer.h"

int IsEnd(LimitedStr * input) {
	return 0 == CurrentChr(input);
}

int IsOpeningBracket(LimitedStr * input) {
	return '(' == CurrentChr(input);
}

int IsEscape(LimitedStr * input) {
	return '\'' == CurrentChr(input);
}

int IsQuote(LimitedStr * input) {
	return '\"' == CurrentChr(input);
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

void SkipQuotedString(LimitedStr * input) {
	while (!IsEnd(input) && !IsQuote(input))
		IterateChr(input);
}

Token GetToken(LimitedStr * input) {
	Token result;
	unsigned int delta = 0, offset = 0;
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
		else if (IsEscape(input))	{
			result.tag = tokEscape;
			IterateChr(input);
		}
		else if (IsQuote(input))	{
			result.tag = tokQuotedString;
			IterateChr(input);
			SkipQuotedString(input);
			if (IsQuote(input)) {
				IterateChr(input);
				delta = 1;
				offset = 1;
			}
			else
				result.tag = tokError;
		}
		else {
			result.tag = tokSymbol;
			SkipSymbol(input);
		}
		result.range.str = result.range.str + offset;
		result.range.size = input->str - result.range.str - delta;
	}
	return result;
}