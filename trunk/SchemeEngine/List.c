#include <assert.h>
#include <stdarg.h>

#include "List.h"
#include "MemoryManager.h"

Term * Nil() {
	return AllocateTerm(terNil);
}

List MakeListFromArray(unsigned int count, Term * terms[]) {
	List result = NULL;
	Pair * iterator = NULL;
	unsigned int i = 0;
	while (i < count)
		result = AppendUsingIterator(result, &iterator, terms[i++]);
	return result;
}

List MakeList(unsigned int count, ...) {
	List result = NULL;
	Pair * iterator = NULL;
	unsigned int i = 0;
	va_list listPointer;
	va_start(listPointer, count);
	while(i++ < count)
		result = AppendUsingIterator(result, &iterator, va_arg(listPointer, Term *));
	return result;
}

Term * Iterate(List * iterator) {
	Term * first;
	Term * next;
	if (NULL == *iterator)
		return NULL;
	first = (*iterator)->first;
	next = (*iterator)->second;
	if(terNil == next->tag)
		*iterator = NULL;
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
	if (NULL != current) {
		current->second = AllocateTerm(terPair);
		current->second->pair = next;
	}
	*iterator = next;
	if (NULL == list)
		return next;
	return list;
}

int TakeArguments(List from, Term * to[], int atLeast, int atMost, Term ** error) {
	Term * current = NULL;
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
	Pair * current = NULL, * last = NULL;
	if (NULL == result)
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