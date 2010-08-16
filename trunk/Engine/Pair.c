#include <assert.h>

#include "Pair.h"
#include "Boolean.h"
#include "List.h"
#include "Constructors.h"
#include "MemoryManager.h"

Term * FunctionCons(List arguments) {
	Term * args[] = {0, 0}, * error = 0;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	return Cons(args[0], args[1]);
}

Term * FunctionCar(List arguments) {
	Term * arg = 0, * result = 0, * error = 0;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (arg->tag != terPair)
		return InvalidArgumentType();
	return arg->pair->first;
}

Term * FunctionCdr(List arguments) {
	Term * arg = 0, * result = 0, * error = 0;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (arg->tag != terPair)
		return InvalidArgumentType();
	return arg->pair->second;
}

Term * FunctionIsNull(List arguments) {
	Term * arg, * error = 0;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (terNil == arg->tag)
		return True();
	return False();
}

Term * FunctionIsPair(List arguments) {
	Term * arg, * error = 0;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (terPair == arg->tag)
		return True();
	return False();
}
