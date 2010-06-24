#include <assert.h>

#include "Number.h"
#include "Boolean.h"
#include "Error.h"

Term * OperatorPlus(List arguments) {
	Term * current = 0;
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
	Term * current = 0;
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

Term * OperatorNumberEq(List arguments) {
	Term * current = 0, * first = 0;
	int argLen = 0, allEquals = 1;
	while (current = IterateList(&arguments))
	{
		argLen++;
		if (current->tag != terNumber)
			return InvalidArgumentType();
		if (!first)
			first = current;
		else if (first->number != current->number)
			allEquals = 0;

	}
	if (argLen < 2)
		return InvalidArgumentCount();
	return allEquals ? True() : False();
}

Term * ParseNumber(LimitedStr symbol) {
	int n = 0;
	Chr c;
	while (c = IterateChr(&symbol))
		n = n * 10 + (c - '0');
	return Number(n);
}

int IsNumber(LimitedStr symbol) {
	Chr c;
	while (c = IterateChr(&symbol)) {
		if (c < '0')
			return 0;
		if (c > '9')
			return 0;
	}
	return 1;
}
