#include "Pair.h"

Term * FunctionCons(List arguments) {
	Term * args[] = {0, 0}, * result = 0, * error = 0;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	result = AllocateTerm(tagPair);	
	result->pair = AllocatePair();
	result->pair->first = args[0];
	result->pair->second = args[1];
	return result;
}

Term * FunctionCar(List arguments) {
	Term * arg = 0, * result = 0, * error = 0;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (arg->tag != tagPair)
		return InvalidArgumentType();
	return arg->pair->first;
}

Term * FunctionCdr(List arguments) {
	Term * arg = 0, * result = 0, * error = 0;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (arg->tag != tagPair)
		return InvalidArgumentType();
	return arg->pair->second;
}