#include <assert.h>

#include "Parser.h"
#include "Number.h"
#include "Dictionary.h"
#include "Memory.h"

Term * ParseTerm(LimitedStr symbol) {
	if (isNumber(symbol))
		return parseNumber(symbol);
	return Symbol(symbol);
}

Term * InvalidClosingBracket() {
	Term * result = AllocateTerm(terError);
	result->message = LimitedStrFromConstantStr("Invalid closing bracket");
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
		case tokQuotedString:
			term = ConstantString(token.range);
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