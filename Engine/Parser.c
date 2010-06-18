#include <assert.h>

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

Term * Parse(Token token, Context ** context) {
	switch(token.tag) {
		case tokSymbol:
			assert(token.range.size > 0);
			if (isNumber(token.range))
				return parseNumber(token.range);
			return InvalidSymbol();
		default:
			assert(0);
		case tokEnd:
			return 0;
	}
}

Context * AcquireContext() {
	return 0;
}