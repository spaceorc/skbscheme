#include "Boolean.h"
#include "Error.h"
#include "Character.h"

Term * ParseCharacter(LimitedStr symbol) {
	return Character(symbol.str[2]);
}

int IsCharacter(LimitedStr symbol) {
	return symbol.size >= 4 && '#' == symbol.str[0] && '\\' == symbol.str[1] && 0 != symbol.str[2] && 0 == symbol.str[3];
}

Term * FunctionIsCharacter(List arguments) {
	Term * arg, * error = 0;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (terCharacter == arg->tag)
		return True();
	return False();
}

Term * FunctionCharacterEq(List arguments) {
	Term * current = 0, * first = 0;
	int argLen = 0, allEquals = 1;
	while (current = IterateList(&arguments))
	{
		argLen++;
		if (current->tag != terCharacter)
			return InvalidArgumentType();
		if (!first)
			first = current;
		else if (first->character != current->character)
			allEquals = 0;

	}
	if (argLen < 2)
		return InvalidArgumentCount();
	return allEquals ? True() : False();
}