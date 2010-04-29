#include <malloc.h>

#include "Term.h"

const int tagNumber = 0;
const int tagFunction = 1;
const int tagPair = 2;
const int tagError = 3;
const int tagNil = 4;

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

Term * Cons(Term * first, Term * second) {
	Term * result = AllocateTerm(tagPair);
	result->pair = AllocatePair();
	result->pair->first = first;
	result->pair->second = second;
	return result;
}

Term * CarPair(Pair * pair) {
	return pair->first;
}

Term * CdrPair(Pair * pair) {
	return pair->second;
}

Term * Nil() {
	return AllocateTerm(tagNil);
}