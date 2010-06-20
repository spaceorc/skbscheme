#pragma once

#include "Tokenizer.h"
#include "Redex.h"

Term * Parse(Token token, ParserContext ** context);
ParserContext * AcquireParserContext();
int CanFinishParsing(ParserContext * context);