#include <assert.h>

#include "Term.h"
#include "Memory.h"
#include "Error.h"

Term * IterateList(List * iterator) {
	Term * first;
	Term * next;
	if (0 == *iterator)
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

List AppendList(List list, Pair ** iterator, Term * term) {
	Pair * current = (*iterator);
	Pair * next = AllocatePair();
	next->first = term;
	next->second = Nil();
	if (0 != current) {
		current->second = AllocateTerm(terPair);
		current->second->pair = next;
	}
	*iterator = next;
	if (0 == list)
		return next;
	return list;
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