#include <malloc.h>
#include <assert.h>

#include "Term.h"

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

Term * InvalidArgumentCount() {
	Term * result = AllocateTerm(terError);
	result->message = LimitedStrFromConstantLimitedStr(LimitConstantStr("Invalid argument count"));
	return result;
}

Term * InvalidArgumentType() {
	Term * result = AllocateTerm(terError);
	result->message = LimitedStrFromConstantLimitedStr(LimitConstantStr("Invalid argument type"));
	return result;
}

Term * IterateList(List * iterator) {
	Term * first;
	Term * next;
	if (*iterator == 0)
		return 0;
	first = (*iterator)->first;
	next = (*iterator)->second;
	if(terNil == next->tag)
		*iterator = 0;
	else {
		assert(next->tag == terPair);
		*iterator = next->pair;
	}
	return first;
}

int TakeArguments(List from, Term * to[], int atLeast, int atMost, Term ** error) {
	Term * current;
	int argLen = 0;
	while (current = IterateList(&from)) {
		if (argLen == atMost) {
			*error = InvalidArgumentCount();
			return -1;
		}
		if (terError == current->tag) {
			*error = current;
			return -1;
		}
		to[argLen++] = current;
	}
	if (argLen < atLeast) {
		*error = InvalidArgumentCount();
		return -1;
	}
	return argLen;
}

ContextBindings * AllocateContextBindings(ContextBindings * previous) {
	ContextBindings * result = malloc(sizeof(ContextBindings));
	result->dictionary = 0;
	result->previous = previous;
	return result;
}


