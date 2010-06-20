#include <assert.h>
#include <malloc.h>
#include <memory.h>

#include "Redex.h"
#include "Dictionary.h"
#include "Number.h"
#include "Pair.h"

const char * globalNames [] = {"+", "-", "cons", "car", "cdr"};
FunctionPtr globalPointers [] = {OperatorPlus, OperatorMinus, FunctionCons, FunctionCar, FunctionCdr};

ContextBindings * AllocateContextBindings() {
	ContextBindings * result = malloc(sizeof(ContextBindings));
	result->dictionary = 0;
	result->previous = 0;
	return result;
}

ContextBindings * AcquireContextBindings() {
	ContextBindings * result = AllocateContextBindings();
	int len = sizeof(globalNames)/sizeof(globalNames[0]);
	assert(len == (sizeof(globalPointers)/sizeof(globalPointers[0])));
	while (len-- > 0)
		result->dictionary = InternalSet(result->dictionary, globalNames[len], Function(globalPointers[len]));
	return result;
}

Term * InvalidSymbol() {
	Term * result = AllocateTerm(terError);
	result->message = "Invalid symbol";
	return result;
}

Term * Resolve(ContextBindings * contextBindings, ConstLimitedStr symbol) {
	Term * result = InternalFind(contextBindings->dictionary, symbol);
	if (result == 0)
		return InvalidSymbol();
	return result;
}

List EvalList(List list, ContextBindings * contextBindings) {
	List result = 0;
	Pair * current = 0;
	Term * i = 0;
	while(i = IterateList(&list)) {
		Pair * next = AllocatePair();
		next->first = Eval(i, contextBindings);
		next->second = Nil();
		if (current != 0) {
			current->second = AllocateTerm(terPair);
			current->second->pair = next;
		}
		current = next;
		if (result == 0)
			result = current;
	}
	return result;
}

Term * InternalApply(List arguments, ContextBindings * contextBindings) {
	Term * function = IterateList(&arguments);
	if (0 == function)
		return InvalidArgumentCount();
	if (terRedex == function->tag)
		function = Eval(function, contextBindings);
	if (terSymbol == function->tag)
		function = Resolve(contextBindings, function->symbol);
	switch(function->tag) {
		case terFunction:
			return function->function(EvalList(arguments, contextBindings));
//		case terLazyFunction:
//			return function->lazyFunction(arguments, contextBindings);
		default:
			return InvalidArgumentType();
	}
}

Term * Eval(Term * term, ContextBindings * contextBindings) {
	if (term->tag != terRedex)
		return term;
	return InternalApply(term->redex, contextBindings);
}