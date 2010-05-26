#include "Pair.h"

Term * FunctionCons(Pair * arguments) {
	Term * result = AllocateTerm(tagPair);
	result->pair = AllocatePair();
	result->pair->first = Number(10);
	result->pair->second = Number(20);
	return result;
}