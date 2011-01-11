#pragma once

#include "Term.h"
#include "Parser.h"

Term * AllocateTerm(int tag);
Pair * AllocatePair();
ContextBindings * AllocateContextBindings(ContextBindings * previous);
Chr * AllocateString(unsigned int size);
ParserContext * AllocateParserContext(ParserContext * previous);
void * AllocateEvaluationContext(unsigned int size);