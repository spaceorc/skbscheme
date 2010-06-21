#include <assert.h>

#include "Pair.h"
#include "Memory.h"

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

List InternalAppend(List list, Term * term) {
	// todo ??? maybe term->tag(error) cannot be added?
	List result = list;
	Pair * current, * last;
	if (0 == result)
		current = result = AllocatePair();
	else {
		last = list;
		while (terNil != last->second->tag) {
			assert(terPair == last->second->tag);
			last = last->second->pair;
		}
		current = AllocatePair();
		last->second->tag = terPair;
		last->second->pair = current;
	}
	current->first = term;
	current->second = Nil();
	return result;
}