#include "List.h"
#include "Boolean.h"
#include "Void.h"

Term * Void() {
	return AllocateTerm(terVoid);
}

Term * FunctionIsid(List arguments) {
	Term * arg = NULL, * error = NULL;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (terVoid == arg->tag)
		return True();
	return False();
}

Term * FunctionVoid(List arguments) {
	return Void();
}