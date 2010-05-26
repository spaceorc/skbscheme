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
	int n = 0, argLen = 0;
	while (current = IterateList(&arguments))
	{
		argLen++;
		if (current->tag != tagNumber)
			return InvalidArgumentType();
		n += current->number;
	}
	if (argLen < 2)
		return InvalidArgumentCount();
	return Number(n);
}

Term * OperatorMinus(Pair * arguments) {
	Term * current;
	int n = 0, argLen = 0;
	while (current = IterateList(&arguments)) {
		argLen++;
		if (argLen == 2)
			n = -n;
		if (current->tag != tagNumber)
			return InvalidArgumentType();
		n -= current->number;
	}
	if (argLen < 1)
		return InvalidArgumentCount();
	return Number(n);
}
