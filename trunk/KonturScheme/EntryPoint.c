#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>

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
		case terNil:
			fprintf(file, "nil\n");
			break;
		case terError:
			fprintf(file, "error: %s\n", term->message);
			break;
		case terFunction:
		case terLazyFunction:
		case terDefinedFunction:
			fprintf(file, "function\n");
			break;
		default:
			assert(0);
	}
}

void main() {
	Token token;
	Term * term;
	ParserContext * context = AcquireParserContext();
	ContextBindings * contextBindings = AcquireContextBindings();
	while (1) {
		LimitedStr str = AllocateLimitedStr(1024);
		fprintf(stdout, "scheme> ");
		if (!gets_s(str.str, str.size))
			break;
		while (tokEnd != (token = GetToken(&str)).tag) {
			if (term = Parse(token, &context))
				InternalWrite(stdout, Eval(term, contextBindings));
		}
	}
}