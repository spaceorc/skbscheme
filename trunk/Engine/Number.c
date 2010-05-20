#include "Number.h"

Term * Number(int number) {
	Term * result = AllocateTerm(tagNumber);
	result->number = number;
	return result;
}

Term * IterateList(Pair ** iterator) {
	Term * first;
	Term * next;
	if (*iterator == 0)
		return 0;
	first = CarPair(*iterator);
	next = CdrPair(*iterator);
	if(next->tag == tagNil)
		*iterator = 0;
	else if (next->tag != tagPair)
		return InvalidArgumentType();
	else
		*iterator = next->pair;
	return first;
}

Term * OperatorPlus(Pair * arguments) {
	Term * current;
	int n = 0;
	while (current = IterateList(&arguments))
	{
		if (current->tag != tagNumber)
			return InvalidArgumentType();
		n += current->number;
	}
	return Number(n);
}

Term * OperatorMinus(Pair * arguments) {
	return Nil();
}
