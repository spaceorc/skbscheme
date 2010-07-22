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
	return ConstantString(result);
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
	return Nil();
}

Term * FunctionSubstring(List arguments) {
	return Nil();
}