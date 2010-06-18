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
		case tagRedex:
			return "Redex";
		default:
			assert(0);
			return 0;
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

Term * InvalidSymbol() {
	Term * result = AllocateTerm(tagError);
	result->message = "Invalid symbol";
	return result;
}

Term * Nil() {
	return AllocateTerm(tagNil);
}

Term * IterateList(List * iterator) {
	Term * first;
	Term * next;
	if (*iterator == 0)
		return 0;
	first = (*iterator)->first;
	next = (*iterator)->second;
	if(tagNil == next->tag)
		*iterator = 0;
	else {
		assert(next->tag == tagPair);
		*iterator = next->pair;
	}
	return first;
}

List AppendListElement(List list, Term * term) {
	List result = list;
	Pair * current, * last;
	if (0 == result)
		current = result = AllocatePair();
	else {
		last = list;
		while (tagNil != last->second->tag) {
			assert(tagPair == last->second->tag);
			last = last->second->pair;
		}
		current = AllocatePair();
		last->second->tag = tagPair;
		last->second->pair = current;
	}
	current->first = term;
	current->second = Nil();
	return result;
}

int TakeArguments(List from, Term * to[], int atLeast, int atMost, Term ** error) {
	Term * current;
	int argLen = 0;
	while (current = IterateList(&from)) {
		if (argLen == atMost) {
			*error = InvalidArgumentCount();
			return -1;
		}
		if (tagError == current->tag) {
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

Term * Function(FunctionPtr function) {
	Term * result = AllocateTerm(tagFunction);
	result->function = function;
	return result;
}