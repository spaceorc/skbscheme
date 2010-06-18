#include <malloc.h>
#include <assert.h>

#include "Term.h"

const char * DumpTag(int tag) {
	switch(tag) {
		case terNumber:
			return "Number";
		case terFunction:
			return "Function";
		case terPair:
			return "Pair";
		case terError:
			return "Error";
		case terNil:
			return "Nil";
		case terRedex:
			return "Redex";
		case terConstantString:
			return "Constant string";
		case terSymbol:
			return "Symbol";
		default:
			assert(0);
			return 0;
	}
}

Term * AllocateTerm(int tag) {
	Term * result = malloc(sizeof(Term));
	result->tag = tag;
	result->number = 0;
	return result;
}

Pair * AllocatePair() {
	Pair * result = malloc(sizeof(Pair));
	result->first = 0;
	result->second = 0;
	return result;
}

Term * InvalidArgumentCount() {
	Term * result = AllocateTerm(terError);
	result->message = "Invalid argument count";
	return result;
}

Term * InvalidArgumentType() {
	Term * result = AllocateTerm(terError);
	result->message = "Invalid argument type";
	return result;
}

Term * Nil() {
	return AllocateTerm(terNil);
}

Term * IterateList(List * iterator) {
	Term * first;
	Term * next;
	if (*iterator == 0)
		return 0;
	first = (*iterator)->first;
	next = (*iterator)->second;
	if(terNil == next->tag)
		*iterator = 0;
	else {
		assert(next->tag == terPair);
		*iterator = next->pair;
	}
	return first;
}


int TakeArguments(List from, Term * to[], int atLeast, int atMost, Term ** error) {
	Term * current;
	int argLen = 0;
	while (current = IterateList(&from)) {
		if (argLen == atMost) {
			*error = InvalidArgumentCount();
			return -1;
		}
		if (terError == current->tag) {
			*error = current;
			return -1;
		}
		to[argLen++] = current;
	}
	if (argLen < atLeast) {
		*error = InvalidArgumentCount();
		return -1;
	}
	return argLen;
}

Term * Function(FunctionPtr function) {
	Term * result = AllocateTerm(terFunction);
	result->function = function;
	return result;
}

Term * ConstantString(ConstStr str) {
	return ConstantStringFromLimited(LimitConstStr(str));
}

Term * ConstantStringFromLimited(ConstLimitedStr str) {
	Term * result = AllocateTerm(terConstantString);
	result->constStr = str;
	return result;
}

Term * Symbol(ConstStr str) {
	return SymbolFromLimited(LimitConstStr(str));
}

Term * SymbolFromLimited(ConstLimitedStr str) {
	Term * result = AllocateTerm(terSymbol);
	result->symbol = str;
	return result;
}
