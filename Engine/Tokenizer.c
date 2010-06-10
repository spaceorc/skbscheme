#include <assert.h>

#include "Tokenizer.h"

Token GetToken(ConstStr * input) {
	Token result;
	result.tag = tokEnd;
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