#include <assert.h>

#include "StringBuilder.h"
#include "Tokenizer.h"

typedef void * (*TokenizerState)(Chr chr, StringBuilder * stringBuilder, int * tag);

void * FinishState(Chr chr, StringBuilder * stringBuilder, int * tag) {
	assert(0);
	return 0;
}

void * FinishAndUnwindState(Chr chr, StringBuilder * stringBuilder, int * tag) {
	assert(0);
	return 0;
}

void * ErrorState(Chr chr, StringBuilder * stringBuilder, int * tag) {
	assert(0);
	return 0;
}

void * StringState(Chr chr, StringBuilder * stringBuilder, int * tag);

void * QuoteState(Chr chr, StringBuilder * stringBuilder, int * tag) {
	if (chr < 32)
		return ErrorState;
	*stringBuilder = AppendChr(*stringBuilder, chr);	
	return StringState;
}

void * VariableState(Chr chr, StringBuilder * stringBuilder, int * tag) {
	switch(chr) {
		case '(':
		case ')':
		case '"':
			return FinishAndUnwindState;
		default:
			if (chr <= 32)
				return FinishState;
			*stringBuilder = AppendChr(*stringBuilder, chr);
			return VariableState;
	}
}

void * StringState(Chr chr, StringBuilder * stringBuilder, int * tag) {
	switch(chr) {
		case 0:
			return ErrorState;
		case '\\':
			return QuoteState;
		case '"':
			return FinishState;
		default:
			*stringBuilder = AppendChr(*stringBuilder, chr);	
			return StringState;
	}
}

void * BeginState(Chr chr, StringBuilder * stringBuilder, int * tag) {
	switch(chr) {
		case 0:
			return FinishState;
		case '(':
			*stringBuilder = AppendChr(*stringBuilder, chr);
			*tag = tokOpeningBracket;
			return FinishState;
		case ')':
			*stringBuilder = AppendChr(*stringBuilder, chr);
			*tag = tokClosingBracket;
			return FinishState;
		case '"':
			*tag = tokQuotedString;
			return StringState;
		default:
			if (chr <= 32)
				return BeginState;
			*stringBuilder = AppendChr(*stringBuilder, chr);
			*tag = tokVariable;
			return VariableState;
	}
	return 0;
}

Token GetToken(LimitedStr * input) {
	Token result;
	StringBuilder stringBuilder = AllocateStringBuilder();
	TokenizerState state = BeginState;
	result.tag = tokEnd;
	while (state != FinishState && state != ErrorState && state != FinishAndUnwindState)
		state = state(IterateChr(input), &stringBuilder, &(result.tag));
	if (state == ErrorState)
		result.tag = tokError;
	if (state == FinishAndUnwindState)
		UnwindChr(input);
	result.text = BuildString(stringBuilder);
	return result;

}