#include "Boolean.h"
#include "MemoryManager.h"
#include "Redex.h"
#include "Error.h"

Term * True() {
	Term * result = AllocateTerm(terBoolean);
	result->boolean = 1;
	return result;
}

Term * False() {
	Term * result = AllocateTerm(terBoolean);
	result->boolean = 0;
	return result;
}

int IsTrue(Term * term) {
	return !IsFalse(term);
}

int IsFalse(Term * term) {
	return term->tag == terBoolean && !term->boolean;
}

Term * LazyFunctionAnd(List arguments, ContextBindings * contextBindings) {
	Term * args[] = {0, 0}, * error = 0, * first;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	first = Eval(args[0], contextBindings);
	if (terError == first->tag || IsFalse(first))
		return first;
	return Eval(args[1], contextBindings);
}

Term * LazyFunctionOr(List arguments, ContextBindings * contextBindings) {
	Term * args[] = {0, 0}, * error = 0, * first;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	first = Eval(args[0], contextBindings);
	if (terError == first->tag || IsTrue(first))
		return first;
	return Eval(args[1], contextBindings);
}

Term * LazyFunctionIf(List arguments, ContextBindings * contextBindings) {
	Term * args[] = {0, 0, 0}, * error = 0, * condition;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	condition = Eval(args[0], contextBindings);
	if (terError == condition->tag)
		return condition;
	if (IsTrue(condition))
		return Eval(args[1], contextBindings);
	return Eval(args[2], contextBindings);
}

Term * LazyFunctionCond(List arguments, ContextBindings * contextBindings) {
	Term * condItem = 0, * condition = 0;
	List condItemArguments = 0;
	while (condItem = IterateList(&arguments)) {
		if (terRedex != condItem->tag)
			return BadSyntax();
		condItemArguments = condItem->redex;
		condition = IterateList(&condItemArguments);
		if (!condition)
			return BadSyntax();
		condition = Eval(condition, contextBindings);
		if (terError == condition->tag)
			return condition;
		if (IsTrue(condition))
			return condItemArguments ? EvalList(condItemArguments, contextBindings) : condition;
	}
	return Empty();
}