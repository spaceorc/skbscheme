#pragma once

#include "Tokenizer.h"
#include "Term.h"

typedef struct structParserContext ParserContext;
struct structParserContext {
	List redex;
	ParserContext * previous;
};

Term * Parse(Token token, ParserContext ** context);
ParserContext * AcquireParserContext();
int CanFinishParsing(ParserContext * context);