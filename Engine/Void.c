#include "Boolean.h"
#include "Void.h"

Term * FunctionIsVoid(List arguments) {
	Term * arg, * error = 0;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (terVoid == arg->tag)
		return True();
	return False();
}

Term * FunctionVoid(List arguments) {
	return Void();
}