#include <assert.h>

#include "Parser.h"

#include "Number.h"
#include "Character.h"
#include "Dictionary.h"
#include "List.h"
#include "String.h"
#include "Variable.h"
#include "MemoryManager.h"

Term * ParseTerm(LimitedStr variable) {
	if (IsNumber(variable))
		return ParseNumber(variable);
	if (IsCharacter(variable))
		return ParseCharacter(variable);
	return Variable(variable);
}

Term * Parse(Token token, ParserContext ** context) {
	Term * term = NULL;
	switch(token.tag) {
		case tokVariable:
			assert(token.text.size > 0);
			term = ParseTerm(token.text);
			if (term->tag == terError)
				break;
			assert(term->tag != terRedex);
			if (NULL == *context)
				break;
			(*context)->redex = Append((*context)->redex, term);
			term = NULL;
			break;
		case tokQuotedString:
			term = String(token.text);
			if (NULL == *context)
				break;
			(*context)->redex = Append((*context)->redex, term);
			term = NULL;
			break;
		case tokOpeningBracket:
			*context = AllocateParserContext(*context);
			break;
		case tokClosingBracket:
			if (NULL == *context)
				return InvalidClosingBracket();
			term = AllocateTerm(terRedex);
			term->redex = (*context)->redex;
			*context = (*context)->previous;
			if (NULL == *context)
				break;
			(*context)->redex = Append((*context)->redex, term);
			term = NULL;
			break;
		case tokError:
			term = AllocateTerm(terError);
			term->message = token.text;
			break;
		default:
			assert(0);
	}
	return term;
}

ParserContext * AcquireParserContext() {
	return NULL;
}

int CanFinishParsing(ParserContext * context) {
	return NULL == context;
}