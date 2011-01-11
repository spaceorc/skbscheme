#include <assert.h>

#include "Redex.h"
#include "List.h"
#include "MemoryManager.h"
#include "Dictionary.h"

Term * Redex(List redex) {
	Term * result = AllocateTerm(terRedex);
	result->redex = redex;
	return result;
}

Term * Resolve(ContextBindings * contextBindings, LimitedStr variable) {
	Term * result = NULL;
	if (NULL == contextBindings)
		return InvalidVariable(variable);
	result = Find(contextBindings->dictionary, variable);
	if (NULL == result)
		return Resolve(contextBindings->previous, variable);
	return result;
}

int EvalArguments(List * arguments, ContextBindings * contextBindings, Term ** error) {
	List result = NULL;
	Pair * current = NULL;
	Term * i = NULL;
	while(i = Iterate(arguments)) {
		Pair * next = AllocatePair();
		next->first = EvalRecursive(i, contextBindings);
		if (terError == next->first->tag) {
			*error = next->first;
			return 0;
		}
		next->second = Nil();
		if (NULL != current) {
			current->second = AllocateTerm(terPair);
			current->second->pair = next;
		}
		current = next;
		if (NULL == result)
			result = current;
	}
	*arguments = result;
	return 1;
}

Term * EvalList(List list, ContextBindings * contextBindings) {
	Term * result = NULL;
	Term * i = NULL;
	while(i = Iterate(&list))
		EvalTermAndCheckError(result, i, contextBindings);
	if (!result)
		return InvalidArgumentCount();
	return result;
}

static Term * EvalLambda(Lambda lambda, List arguments) {
	Term * formalArgument = NULL, * argument = NULL;
	ContextBindings * childContextBindings = AllocateContextBindings(lambda.context);
	while(formalArgument = Iterate(&lambda.formalArguments)) {
		argument = Iterate(&arguments);
		if (!argument)
			return InvalidArgumentCount();
		CheckTermType(formalArgument, terVariable);
		childContextBindings->dictionary = Set(childContextBindings->dictionary, formalArgument->variable, argument);
	}
	if (Iterate(&arguments))
		return InvalidArgumentCount();
	return EvalList(lambda.body, childContextBindings);
}

Term * InternalApply(List arguments, ContextBindings * contextBindings) {
	Term * function = Iterate(&arguments), * error = NULL;
	if (NULL == function)
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