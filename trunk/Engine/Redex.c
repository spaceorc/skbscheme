#include <assert.h>
#include <malloc.h>
#include <memory.h>

#include "Redex.h"
#include "Dictionary.h"
#include "Number.h"
#include "Pair.h"
#include "Definitions.h"

const char * globalFunctionNames [] = {"+", "-", "cons", "car", "cdr"};
FunctionPtr globalFunctionPointers [] = {OperatorPlus, OperatorMinus, FunctionCons, FunctionCar, FunctionCdr};
const char * globalLazyFunctionNames [] = {"let", "define"};
LazyFunctionPtr globalLazyFunctionPointers [] = {LazyFunctionLet, LazyFunctionDefine};

ContextBindings * AllocateContextBindings() {
	ContextBindings * result = malloc(sizeof(ContextBindings));
	result->dictionary = 0;
	result->previous = 0;
	return result;
}

ContextBindings * AcquireContextBindings() {
	ContextBindings * result = AllocateContextBindings();
	int lenFunctions = sizeof(globalFunctionNames)/sizeof(globalFunctionNames[0]);
	int lenLazyFunctions = sizeof(globalLazyFunctionNames)/sizeof(globalLazyFunctionNames[0]);
	assert((sizeof(globalFunctionPointers)/sizeof(globalFunctionPointers[0])) == lenFunctions);
	while (lenFunctions-- > 0)
		result->dictionary = InternalSet(result->dictionary, globalFunctionNames[lenFunctions], Function(globalFunctionPointers[lenFunctions]));
	assert((sizeof(globalLazyFunctionPointers)/sizeof(globalLazyFunctionPointers[0])) == lenLazyFunctions);
	while (lenLazyFunctions-- > 0)
		result->dictionary = InternalSet(result->dictionary, globalLazyFunctionNames[lenLazyFunctions], LazyFunction(globalLazyFunctionPointers[lenLazyFunctions]));
	return result;
}

Term * InvalidSymbol() {
	Term * result = AllocateTerm(terError);
	result->message = "Invalid symbol";
	return result;
}

Term * Resolve(ContextBindings * contextBindings, ConstLimitedStr symbol) {
	Term * result = InternalFind(contextBindings->dictionary, symbol);
	if (0 == result)
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
		if (0 != current) {
			current->second = AllocateTerm(terPair);
			current->second->pair = next;
		}
		current = next;
		if (0 == result)
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
		case terLazyFunction:
			return function->lazyFunction(arguments, contextBindings);
		default:
			return InvalidArgumentType();
	}
}

Term * Eval(Term * term, ContextBindings * contextBindings) {
	switch (term->tag) {
		case terRedex:
			return InternalApply(term->redex, contextBindings);
		case terSymbol:
			return Eval(Resolve(contextBindings, term->symbol), contextBindings);
		default:
			return term;
	}
}