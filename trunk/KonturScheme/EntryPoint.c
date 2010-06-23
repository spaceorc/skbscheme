#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "Tokenizer.h"
#include "Parser.h"
#include "Redex.h"

int InternalWrite(FILE * file, Term * term);

void InternalWritePair(FILE * file, Pair * pair) {
	char delimiter = '(';
	while(pair) {
		fputc(delimiter, file);
		delimiter = ' ';
		InternalWrite(file, pair->first);
		switch(pair->second->tag) {
			case terPair:
				pair = pair->second->pair;
				break;
			case terNil:
				pair = 0;
				break;
			default:
				fprintf(file, " . ");
				InternalWrite(file, pair->second);
				pair = 0;
				break;
		}
	}
	fputc(')', file);
}

int InternalWrite(FILE * file, Term * term) {
	switch(term->tag) {
		case terEmpty:
			return 0;
		case terNumber:
			fprintf(file, "%d", term->number);
			break;
		case terCharacter:
			fprintf(file, "%c", (int)term->character);
			break;
		case terSymbol:
		case terConstantString:
			fprintf(file, "%.*s", term->symbol.size, term->symbol.str);
			break;
		case terNil:
			fprintf(file, "()");
			break;
		case terError:
			fprintf(file, "error: %.*s", term->message.size, term->message.str);
			break;
		case terFunction:
		case terLazyFunction:
		case terDefinedFunction:
			fprintf(file, "function");
			break;
		case terBoolean:
			if (term->boolean)
				fprintf(file, "#t");
			else
				fprintf(file, "#f");
			break;
		case terPair:
			InternalWritePair(file, term->pair);
			break;
		default:
			assert(0);
	}
	return 1;
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
			if (term = Parse(token, &context)) {
				if (InternalWrite(stdout, Eval(term, contextBindings)))
					fprintf(stdout, "\n");
			}
		}
	}
}