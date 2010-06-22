#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "Tokenizer.h"
#include "Parser.h"
#include "Redex.h"

void InternalWrite(FILE * file, Term * term) {
	switch(term->tag) {
		case terEmpty:
			break;
		case terNumber:
			fprintf(file, "%d\n", term->number);
			break;
		case terSymbol:
		case terConstantString:
			fprintf(file, "%.*s\n", term->symbol.size, term->symbol.str);
			break;
		case terNil:
			fprintf(file, "()\n");
			break;
		case terError:
			fprintf(file, "error: %s\n", term->message);
			break;
		case terFunction:
		case terLazyFunction:
		case terDefinedFunction:
			fprintf(file, "function\n");
			break;
		case terBoolean:
			if (term->boolean)
				fprintf(file, "#t\n");
			else
				fprintf(file, "#f\n");
			break;
		default:
			assert(0);
	}
}

void Prompt(FILE * file) {
	fprintf(file, "> ");
}

void main() {
	Token token;
	Term * term;
	ParserContext * context = AcquireParserContext();
	ContextBindings * contextBindings = AcquireContextBindings();
	while (1) {
		LimitedStr str = AllocateLimitedStr(1024);
		if (CanFinishParsing(context))
			Prompt(stdout);
		if (!gets_s(str.str, str.size))
			break;
		while (tokEnd != (token = GetToken(&str)).tag) {
			if (term = Parse(token, &context))
				InternalWrite(stdout, Eval(term, contextBindings));
		}
	}
}