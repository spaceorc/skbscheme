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
		case terLazyFunction:
			return "Lazy function";
		case terDefinedFunction:
			return "Defined function";
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
	result->message = LimitedStrFromConstantLimitedStr(LimitConstantStr("Invalid argument count"));
	return result;
}

Term * InvalidArgumentType() {
	Term * result = AllocateTerm(terError);
	result->message = LimitedStrFromConstantLimitedStr(LimitConstantStr("Invalid argument type"));
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

Term * LazyFunction(LazyFunctionPtr lazyFunction) {
	Term * result = AllocateTerm(terLazyFunction);
	result->lazyFunction = lazyFunction;
	return result;
}

Term * DefineFunction(List formalArguments, Term * function) {
	Term * result = AllocateTerm(terDefinedFunction);
	result->definedFunction.formalArguments = formalArguments;
	result->definedFunction.function = function;
	return result;
}

Term * ConstantStringFromConstantStr(ConstantStr str) {
	return ConstantStringFromConstantLimitedStr(LimitConstantStr(str));
}

Term * ConstantStringFromConstantLimitedStr(ConstantLimitedStr str) {
	return ConstantString(LimitedStrFromConstantLimitedStr(str));
}

Term * ConstantString(LimitedStr str) {
	Term * result = AllocateTerm(terConstantString);
	result->constantString = str;
	return result;
}

Term * Symbol(LimitedStr str) {
	Term * result = AllocateTerm(terSymbol);
	result->symbol = str;
	return result;
}

Term * SymbolFromConstantStr(ConstantStr str) {
	return SymbolFromConstantLimitedStr(LimitConstantStr(str));
}

Term * SymbolFromConstantLimitedStr(ConstantLimitedStr str) {
	return Symbol(LimitedStrFromConstantLimitedStr(str));
}

ContextBindings * AllocateContextBindings(ContextBindings * previous) {
	ContextBindings * result = malloc(sizeof(ContextBindings));
	result->dictionary = 0;
	result->previous = previous;
	return result;
}