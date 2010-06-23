#include "Error.h"

Term * InvalidSymbol(LimitedStr symbol) {
	return Error(ConcatenateConstantLimitedStr(LimitConstantStr("invalid symbol "), ConstLimitedStr(symbol)));
}

Term * InvalidClosingBracket() {
	return Error(LimitedStrFromConstantStr("Invalid closing bracket"));
}

Term * InvalidArgumentCount() {
	return Error(LimitedStrFromConstantStr("Invalid argument count"));
}

Term * InvalidArgumentType() {
	return Error(LimitedStrFromConstantStr("Invalid argument type"));
}

Term * BadSyntax() {
	return Error(LimitedStrFromConstantStr("Bad syntax"));
}

Term * FunctionError(List arguments) {
	Term * message = 0, * error = 0;
	if (TakeSingleArgument(arguments, &message, &error) < 0)
		return error;
	if (terConstantString != message->tag)
		return InvalidArgumentType();
	return Error(message->constantString);
}