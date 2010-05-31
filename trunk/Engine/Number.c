#include <assert.h>

#include "Number.h"

Term * Number(int number) {
	Term * result = AllocateTerm(tagNumber);
	result->number = number;
	return result;
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