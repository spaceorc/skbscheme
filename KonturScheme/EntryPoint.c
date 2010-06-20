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
		default:
			assert(0);
	}
}

void main() {
	char str[1024];
	Token token;
	Term * term;
	ParserContext * context = AcquireParserContext();
	ContextBindings * contextBindings = AcquireContextBindings();
	while (1) {
		char * copy = 0;
		fprintf(stdout, "scheme> ");
		if (!gets_s(str, sizeof(str)/sizeof(char)))
			break;
		copy = malloc(strlen(str) + 1);
		// bug: free copy? when? where?
		strcpy_s(copy, strlen(str) + 1, str);
		while (tokEnd != (token = GetToken(&copy)).tag) {
			if (term = Parse(token, &context))
				InternalWrite(stdout, Eval(term, contextBindings));
		}
	}
}