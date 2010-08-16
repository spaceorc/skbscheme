#include <assert.h>

#include "List.h"
#include "Constructors.h"
#include "MemoryManager.h"

Term * Iterate(List * iterator) {
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

List AppendUsingIterator(List list, Pair ** iterator, Term * term) {
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
	while (current = Iterate(&from)) {
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

List Append(List list, Term * term) {
	// todo ??? maybe term->tag(error) cannot be added?
	List result = list;
	Pair * current, * last;
	if (0 == result)
		current = result = AllocatePair();
	else {
		last = list;
		while (terNil != last->second->tag) {
			assert(terPair == last->second->tag);
			last = last->second->pair;
		}
		current = AllocatePair();
		last->second->tag = terPair;
		last->second->pair = current;
	}
	current->first = term;
	current->second = Nil();
	return result;
}

List Push(List list, Term * term) {
	List result = AllocatePair();
	Term * t = AllocateTerm(terPair);
	t->pair = list;
	result->second = t;
	result->first = term;
	return result;
}