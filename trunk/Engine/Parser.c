#include <assert.h>
#include <malloc.h>

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
	switch(token.tag) {
		case tokSymbol:
			assert(token.range.size > 0);
			if (isNumber(token.range))
				return parseNumber(token.range);
			return InvalidSymbol();
		case tokOpeningBracket:
			*context = AllocateContext(*context);
			return 0;
		case tokClosingBracket:
			if (0 == *context)
				return InvalidClosingBracket();
			*context = (*context)->previous;
			return 0;
		default:
			assert(0);
			return 0;
	}
}

Context * AcquireContext() {
	return 0;
}

int CanFinishParsing(Context * context) {
	return 0 == context;
}