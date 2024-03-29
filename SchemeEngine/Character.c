#include "Character.h"
#include "List.h"
#include "Boolean.h"

Term * Character(Chr character) {
	Term * result = AllocateTerm(terCharacter);
	result->character = character;
	return result;
}

Term * ParseCharacter(LimitedStr variable) {
	return Character(variable.str[2]);
}

int IsCharacter(LimitedStr variable) {
	return variable.size >= 4 && '#' == variable.str[0] && '\\' == variable.str[1] && '\0' != variable.str[2] && '\0' == variable.str[3];
}

Term * FunctionIsCharacter(List arguments) {
	Term * arg = NULL, * error = NULL;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (terCharacter == arg->tag)
		return True();
	return False();
}

Term * FunctionCharacterEq(List arguments) {
	Term * current = NULL, * first = NULL;
	int argLen = 0, allEquals = 1;
	while (current = Iterate(&arguments))
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