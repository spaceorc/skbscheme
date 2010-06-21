#include <malloc.h>
#include <memory.h>
#include <assert.h>

#include "Memory.h"

Term * AllocateTerm(int tag) {
	Term * result = malloc(sizeof(Term));
	memset(result, 0, sizeof(*result));
	result->tag = tag;
	return result;
}

Pair * AllocatePair() {
	Pair * result = malloc(sizeof(Pair));
	memset(result, 0, sizeof(*result));
	return result;
}

ContextBindings * AllocateContextBindings(ContextBindings * previous) {
	ContextBindings * result = malloc(sizeof(ContextBindings));
	memset(result, 0, sizeof(*result));
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
	memset(result, 0, sizeof(*result));
	result->previous = previous;
	return result;
}
