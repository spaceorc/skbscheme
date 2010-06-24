#include <assert.h>

#include "Redex.h"
#include "Memory.h"
#include "Error.h"
#include "Dictionary.h"

Term * Resolve(ContextBindings * contextBindings, LimitedStr symbol) {
	Term * result = 0;
	if (0 == contextBindings)
		return InvalidSymbol(symbol);
	result = InternalFind(contextBindings->dictionary, symbol);
	if (0 == result)
		return Resolve(contextBindings->previous, symbol);
	return result;
}

int EvalArguments(List * arguments, ContextBindings * contextBindings, Term ** error) {
	List result = 0;
	Pair * current = 0;
	Term * i = 0;
	while(i = IterateList(arguments)) {
		Pair * next = AllocatePair();
		next->first = Eval(i, contextBindings);
		if (terError == next->first->tag) {
			*error = next->first;
			return 0;
		}
		next->second = Nil();
		if (0 != current) {
			current->second = AllocateTerm(terPair);
			current->second->pair = next;
		}
		current = next;
		if (0 == result)
			result = current;
	}
	*arguments = result;
	return 1;
}

Term * EvalList(List list, ContextBindings * contextBindings) {
	Term * result = 0;
	Term * i = 0;
	while(i = IterateList(&list)) {
		result = Eval(i, contextBindings);
		if (terError == result->tag)
			break;
	}
	if (!result)
		return InvalidArgumentCount();
	return result;
}

Term * DefinedFunctionApply(DefinedFunction definedFunction, List arguments, ContextBindings * contextBindings) {
	Term * formalArgument = 0, * argument = 0;
	ContextBindings * childContextBindings = AllocateContextBindings(definedFunction.context);
	while(formalArgument = IterateList(&definedFunction.formalArguments)) {
		argument = IterateList(&arguments);
		if (!argument)
			return InvalidArgumentCount();
		if (formalArgument->tag != terSymbol)
			return InvalidArgumentType();
		childContextBindings->dictionary = InternalSet(childContextBindings->dictionary, formalArgument->symbol, argument);
	}
	if (IterateList(&arguments))
		return InvalidArgumentCount();
	return EvalList(definedFunction.body, childContextBindings);
}

Term * InternalApply(List arguments, ContextBindings * contextBindings) {
	Term * function = IterateList(&arguments), * error;
	if (0 == function)
		return InvalidArgumentCount();
	function = Eval(function, contextBindings);
	switch(function->tag) {
		case terFunction:
			if (!EvalArguments(&arguments, contextBindings, &error))
				return error;
			return function->function(arguments);
		case terLazyFunction:
			return function->lazyFunction(arguments, contextBindings);
		case terDefinedFunction:
			if (!EvalArguments(&arguments, contextBindings, &error))
				return error;
			return DefinedFunctionApply(function->definedFunction, arguments, contextBindings);
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