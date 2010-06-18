#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "Parser.h"
#include "Number.h"

char IterateSymbol(ConstLimitedStr * symbol) {
	char result = 0;
	if (symbol->size > 0) {
		result = *(symbol->str);
		symbol->str++;
		symbol->size--;
	}
	return result;
}

Term * parseNumber(ConstLimitedStr symbol) {
	int n = 0;
	char c;
	while (c = IterateSymbol(&symbol))
		n = n * 10 + (c - '0');
	return Number(n);
}

int isNumber(ConstLimitedStr symbol) {
	char c;
	while (c = IterateSymbol(&symbol)) {
		if (c < '0')
			return 0;
		if (c > '9')
			return 0;
	}
	return 1;
}

Term * parseBoundTerm(ConstLimitedStr symbol) {
	if (strncmp("+", symbol.str, 1))
		return InvalidSymbol();
	return Function(OperatorPlus);
}

Term * ParseTerm(ConstLimitedStr symbol) {
	if (isNumber(symbol))
		return parseNumber(symbol);
	return parseBoundTerm(symbol);
}

Context * AllocateContext(Context * previous) {
	Context * result = malloc(sizeof(Context));
	result->previous = previous;
	result->redex = 0;
	return result;
}

Term * InvalidClosingBracket() {
	Term * result = AllocateTerm(tagError);
	result->message = "Invalid closing bracket";
	return result;
}

Term * Parse(Token token, Context ** context) {
	Term * term = 0;
	switch(token.tag) {
		case tokSymbol:
			assert(token.range.size > 0);
			term = ParseTerm(token.range);
			if (term->tag == tagError)
				break;
			assert(term->tag != tagRedex);
			if (0 == *context)
				break;
			(*context)->redex = AppendListElement((*context)->redex, term);
			term = 0;
			break;
		case tokOpeningBracket:
			*context = AllocateContext(*context);
			break;
		case tokClosingBracket:
			if (0 == *context)
				return InvalidClosingBracket();
			term = AllocateTerm(tagRedex);
			term->redex = (*context)->redex;
			*context = (*context)->previous;
			break;
		default:
			assert(0);
	}
	return term;
}

Context * AcquireContext() {
	return 0;
}

int CanFinishParsing(Context * context) {
	return 0 == context;
}