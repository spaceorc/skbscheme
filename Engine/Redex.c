#include "Redex.h"

Term * FunctionInternalApply(List arguments, ContextBindings * contextBindings) {
	Term * function = 0;
	function = IterateList(&arguments);
	if (function == 0)
		return InvalidArgumentCount();
	if (function->tag != terFunction)
		return InvalidArgumentType();
	return function->function(arguments);
}

List ReduceList(List list, ContextBindings * contextBindings) {
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

Term * Eval(Term * term, ContextBindings * contextBindings) {
	if (term->tag != terRedex)
		return term;
	return FunctionInternalApply(ReduceList(term->redex, contextBindings), contextBindings);
}