#include <malloc.h>
#include <assert.h>

#include "Term.h"

const char * DumpTag(int tag) {
	switch(tag) {
		case tagNumber:
			return "Number";
		case tagFunction:
			return "Function";
		case tagPair:
			return "Pair";
		case tagError:
			return "Error";
		case tagNil:
			return "Nil";
		default:
			assert(0);
	}
}

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

void ReleaseTerm(Term * term) {
	free(term);
}

void ReleasePair(Pair * pair) {
	free(pair);
}

Term * InvalidArgumentCount() {
	Term * result = AllocateTerm(tagError);
	result->message = "Invalid argument count";
	return result;
}

Term * InvalidArgumentType() {
	Term * result = AllocateTerm(tagError);
	result->message = "Invalid argument type";
	return result;
}

Term * Nil() {
	return AllocateTerm(tagNil);
}