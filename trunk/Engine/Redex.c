#include <assert.h>

#include "Redex.h"
#include "MemoryManager.h"
#include "Dictionary.h"

Term * Resolve(ContextBindings * contextBindings, LimitedStr variable) {
	Term * result = 0;
	if (0 == contextBindings)
		return InvalidVariable(variable);
	result = Find(contextBindings->dictionary, variable);
	if (0 == result)
		return Resolve(contextBindings->previous, variable);
	return result;
}

int EvalArguments(List * arguments, ContextBindings * contextBindings, Term ** error) {
	List result = 0;
	Pair * current = 0;
	Term * i = 0;
	while(i = IterateList(arguments)) {
		Pair * next = AllocatePair();
		next->first = EvalRecursive(i, contextBindings);
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
	while(i = IterateList(&list))
		EvalTermAndCheckError(result, i, contextBindings);
	if (!result)
		return InvalidArgumentCount();
	return result;
}

static Term * EvalLambda(Lambda lambda, List arguments) {
	Term * formalArgument = 0, * argument = 0;
	ContextBindings * childContextBindings = AllocateContextBindings(lambda.context);
	while(formalArgument = IterateList(&lambda.formalArguments)) {
		argument = IterateList(&arguments);
		if (!argument)
			return InvalidArgumentCount();
		CheckTermType(formalArgument, terVariable);
		childContextBindings->dictionary = Set(childContextBindings->dictionary, formalArgument->variable, argument);
	}
	if (IterateList(&arguments))
		return InvalidArgumentCount();
	return EvalList(lambda.body, childContextBindings);
}

Term * InternalApply(List arguments, ContextBindings * contextBindings) {
	Term * function = IterateList(&arguments), * error;
	if (0 == function)
		return InvalidArgumentCount();
	function = EvalRecursive(function, contextBindings);
	switch(function->tag) {
		case terFunction:
			if (!EvalArguments(&arguments, contextBindings, &error))
				return error;
			return function->function(arguments);
		case terLazyFunction:
			return function->lazy.function(arguments, contextBindings);
		case terLambda:
			if (!EvalArguments(&arguments, contextBindings, &error))
				return error;
			return EvalLambda(function->lambda, arguments);
		default:
			return InvalidArgumentType();
	}
}

Term * EvalRecursive(Term * term, ContextBindings * contextBindings) {
	switch (term->tag) {
		case terRedex:
			return InternalApply(term->redex, contextBindings);
		case terVariable:
			return EvalRecursive(Resolve(contextBindings, term->variable), contextBindings);
		default:
			return term;
	}
}