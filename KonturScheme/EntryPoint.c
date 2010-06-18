#include <stdio.h>
#include <assert.h>

#include "Tokenizer.h"
#include "Parser.h"
#include "Redex.h"

// bug with %*s format
void InternalWrite(FILE * file, Term * term) {
	switch(term->tag) {
		case terNumber:
			fprintf(file, "%d\n", term->number);
			break;
		case terSymbol:
			fprintf(file, "%*s\n", term->symbol.size, term->symbol.str);
			break;
		default:
			assert(0);
	}
}

void main() {
	char str[1024], * offset;
	Token token;
	Term * term;
	ParserContext * context = AcquireParserContext();
	ContextBindings * contextBindings = AcquireContextBindings();
	while (offset = gets_s(str, sizeof(str)/sizeof(char))) {
		while (tokEnd != (token = GetToken(&offset)).tag) {
			if (term = Parse(token, &context))
				InternalWrite(stdout, Eval(term, contextBindings));
		}
	}
}