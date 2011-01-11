#include <assert.h>

#include "Pair.h"
#include "Boolean.h"
#include "List.h"
#include "MemoryManager.h"

Term * Cons(Term * first, Term * second) {
	Term * result = NULL;
	result = AllocateTerm(terPair);	
	result->pair = AllocatePair();
	result->pair->first = first;
	result->pair->second = second;
	return result;
}

Term * FunctionCons(List arguments) {
	Term * args[] = {NULL, NULL}, * error = NULL;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	return Cons(args[0], args[1]);
}

Term * FunctionCar(List arguments) {
	Term * arg = NULL, * result = NULL, * error = NULL;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (arg->tag != terPair)
		return InvalidArgumentType();
	return arg->pair->first;
}

Term * FunctionCdr(List arguments) {
	Term * arg = NULL, * result = NULL, * error = NULL;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (arg->tag != terPair)
		return InvalidArgumentType();
	return arg->pair->second;
}

Term * FunctionIsNull(List arguments) {
	Term * arg = NULL, * error = NULL;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (terNil == arg->tag)
		return True();
	return False();
}

Term * FunctionIsPair(List arguments) {
	Term * arg = NULL, * error = NULL;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (terPair == arg->tag)
		return True();
	return False();
}
