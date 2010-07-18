#pragma once

#include "Term.h"
#include "Str.h"

Term * AllocateTerm(int tag);
Pair * AllocatePair();
ContextBindings * AllocateContextBindings(ContextBindings * previous);
Chr * AllocateString(unsigned int size);
ParserContext * AllocateParserContext(ParserContext * previous);
StringBuilder * AllocateStringBuilder(StringBuilder * next, unsigned int size);
EvaluationContextBase * AllocateEvaluationContext(unsigned int size);