#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "Parser.h"
#include "Number.h"
#include "Dictionary.h"

Term * ParseTerm(ConstLimitedStr symbol) {
	if (isNumber(symbol))
		return parseNumber(symbol);
	return SymbolFromLimited(symbol);
}

ParserContext * AllocateParserContext(ParserContext * previous) {
	ParserContext * result = malloc(sizeof(ParserContext));
	result->previous = previous;
	result->redex = 0;
	return result;
}

Term * InvalidClosingBracket() {
	Term * result = AllocateTerm(terError);
	result->message = "Invalid closing bracket";
	return result;
}

Term * Parse(Token token, ParserContext ** context) {
	Term * term = 0;
	switch(token.tag) {
		case tokSymbol:
			assert(token.range.size > 0);
			term = ParseTerm(token.range);
			if (term->tag == terError)
				break;
			assert(term->tag != terRedex);
			if (0 == *context)
				break;
			(*context)->redex = InternalAppend((*context)->redex, term);
			term = 0;
			break;
		case tokOpeningBracket:
			*context = AllocateParserContext(*context);
			break;
		case tokClosingBracket:
			if (0 == *context)
				return InvalidClosingBracket();
			term = AllocateTerm(terRedex);
			term->redex = (*context)->redex;
			*context = (*context)->previous;
			if (0 == *context)
				break;
			(*context)->redex = InternalAppend((*context)->redex, term);
			term = 0;
			break;
		default:
			assert(0);
	}
	return term;
}

ParserContext * AcquireParserContext() {
	return 0;
}

int CanFinishParsing(ParserContext * context) {
	return 0 == context;
}