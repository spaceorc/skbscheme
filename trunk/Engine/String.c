#include "Boolean.h"
#include "String.h"

Term * FunctionIsString(List arguments) {
	Term * arg, * error = 0;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (terString == arg->tag)
		return True();
	return False();
}

Term * FunctionMakeString(List arguments) {
	Term * args[] = {0, 0}, * error = 0;
	LimitedStr result;
	Chr * chr;
	unsigned int size;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (terNumber != args[0]->tag)
		return InvalidArgumentType();
	if (terCharacter != args[1]->tag)
		return InvalidArgumentType();
	size = args[0]->number;
	result = CreateLimitedStr(size + 1);
	chr = result.str;
	while (size--)
		*(chr++) = args[1]->character;
	*chr = 0;
	return String(result);
}

Term * FunctionStringLength(List arguments) {
	Term * arg = 0, * error = 0;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (terString != arg->tag)
		return InvalidArgumentType();
	return Number(Length(arg->string));
}

Term * FunctionStringRef(List arguments) {
	Term * args [] = {0, 0}, * error  = 0;
	unsigned int pos = 0;
	LimitedStr str;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (terString != args[0]->tag)
		return InvalidArgumentType();
	if (terNumber != args[1]->tag)
		return InvalidArgumentType();
	str = args[0]->string;
	pos = args[1]->number;
	if (pos < 0 || pos >= str.size - 1)
		return ContractError();
	return Character(str.str[pos]);
}

Term * FunctionStringSet(List arguments) {
	Term * args [] = {0, 0, 0}, * error  = 0;
	unsigned int pos = 0;
	LimitedStr str;
	Chr chr;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (terString != args[0]->tag)
		return InvalidArgumentType();
	if (terNumber != args[1]->tag)
		return InvalidArgumentType();
	if (terCharacter != args[2]->tag)
		return InvalidArgumentType();
	str = args[0]->string;
	pos = args[1]->number;
	chr = args[2]->character;
	if (pos < 0 || pos >= str.size - 1)
		return ContractError();
	str.str[pos] = chr;
	return Void();
}

Term * FunctionSubstring(List arguments) {
	Term * args [] = {0, 0, 0}, * error  = 0;
	unsigned int from = 0, to = 0;
	LimitedStr str;
	if (TakeArguments(arguments, args, 2, 3, &error) < 0)
		return error;
	if (terString != args[0]->tag)
		return InvalidArgumentType();
	if (terNumber != args[1]->tag)
		return InvalidArgumentType();
	str = args[0]->string;
	from = args[1]->number;
	if (!args[2]) {
		to = str.size - 1;
		if (!str.str[to])
			to--;
	}
	else if (terNumber != args[2]->tag)
		return InvalidArgumentType();
	else {
		to = args[2]->number;
		if (to < 0 || to >= str.size - 1 || !str.str[to])
			return ContractError();
	}
	if (from < 0 || from > to)
		return ContractError();
	str.str += from;
	str.size = to - from + 1;
	return String(str);
}

Term * FunctionStringEq(List arguments) {
	Term * current = 0, * first = 0;
	int argLen = 0, allEquals = 1;
	while (current = Iterate(&arguments))
	{
		argLen++;
		if (current->tag != terString)
			return InvalidArgumentType();
		if (!first)
			first = current;
		else if (Compare(first->string, current->string))
			allEquals = 0;
	}
	if (argLen < 2)
		return InvalidArgumentCount();
	return allEquals ? True() : False();
}