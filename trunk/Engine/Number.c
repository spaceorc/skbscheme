#include <assert.h>

#include "Number.h"

Term * OperatorPlus(List arguments) {
	Term * current;
	int n = 0, argLen = 0;
	while (current = IterateList(&arguments))
	{
		argLen++;
		if (current->tag != terNumber)
			return InvalidArgumentType();
		n += current->number;
	}
	if (argLen < 2)
		return InvalidArgumentCount();
	return Number(n);
}

Term * OperatorMinus(List arguments) {
	Term * current;
	int n = 0, argLen = 0;
	while (current = IterateList(&arguments)) {
		argLen++;
		if (argLen == 2)
			n = -n;
		if (current->tag != terNumber)
			return InvalidArgumentType();
		n -= current->number;
	}
	if (argLen < 1)
		return InvalidArgumentCount();
	return Number(n);
}

Term * parseNumber(LimitedStr symbol) {
	int n = 0;
	Chr c;
	while (c = IterateChr(&symbol))
		n = n * 10 + (c - '0');
	return Number(n);
}

int isNumber(LimitedStr symbol) {
	Chr c;
	while (c = IterateChr(&symbol)) {
		if (c < '0')
			return 0;
		if (c > '9')
			return 0;
	}
	return 1;
}
