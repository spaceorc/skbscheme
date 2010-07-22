#pragma once

#include "Term.h"

Term * AllocateTerm(int tag);
Pair * AllocatePair();
ContextBindings * AllocateContextBindings(ContextBindings * previous);
Chr * AllocateString(unsigned int size);
ParserContext * AllocateParserContext(ParserContext * previous);
EvaluationContextBase * AllocateEvaluationContext(unsigned int size);