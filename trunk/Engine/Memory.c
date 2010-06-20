#include <malloc.h>
#include <assert.h>

#include "Memory.h"

Term * AllocateTerm(int tag) {
	Term * result = malloc(sizeof(Term));
	result->tag = tag;
	result->number = 0;
	return result;
}

Pair * AllocatePair() {
	Pair * result = malloc(sizeof(Pair));
	result->first = 0;
	result->second = 0;
	return result;
}

ContextBindings * AllocateContextBindings(ContextBindings * previous) {
	ContextBindings * result = malloc(sizeof(ContextBindings));
	result->dictionary = 0;
	result->previous = previous;
	return result;
}

Chr * AllocateString(unsigned int size) {
	Chr * result = malloc(size);
	assert(size > 0);
	result[0] = 0;
	return result;
}

ParserContext * AllocateParserContext(ParserContext * previous) {
	ParserContext * result = malloc(sizeof(ParserContext));
	result->previous = previous;
	result->redex = 0;
	return result;
}
