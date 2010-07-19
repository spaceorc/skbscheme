#include <stdlib.h>

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
	if (terString != message->tag)
		return InvalidArgumentType();
	return Error(message->string);
}

// warning C4996: '__sys_errlist': This function or variable may be unsafe. Consider using strerror instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.	c:\Source\skbscheme\Engine\Error.c	35	Engine
#pragma warning(disable:4996)

Term * RaisePosixError(int errorNumber) {
	return Error(LimitedStrFromConstantStr(_sys_errlist[errorNumber]));
}

#pragma warning(default:4996)