#include "Pair.h"

Term * InternalCons(Term * first, Term * second) {
	Term * result = 0;
	result = AllocateTerm(tagPair);	
	result->pair = AllocatePair();
	result->pair->first = first;
	result->pair->second = second;
	return result;
}

Term * FunctionCons(List arguments) {
	Term * args[] = {0, 0}, * error = 0;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	return InternalCons(args[0], args[1]);
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