#include "Boolean.h"
#include "MemoryManager.h"
#include "Redex.h"

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
	Term * arg, * term = 0;
	while (arg = IterateList(&arguments)) {
		term = EvalRecursive(arg, contextBindings);
		if (terError == term->tag || IsFalse(term))
			return term;
	}
	if (term)
		return term;
	return True();
}

Term * LazyFunctionOr(List arguments, ContextBindings * contextBindings) {
	Term * arg, * term = 0;
	while (arg = IterateList(&arguments)) {
		term = EvalRecursive(arg, contextBindings);
		if (terError == term->tag || IsTrue(term))
			return term;
	}
	if (term)
		return term;
	return False();
}

Term * LazyFunctionIf(List arguments, ContextBindings * contextBindings) {
	Term * args[] = {0, 0, 0}, * error = 0, * condition;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	condition = EvalRecursive(args[0], contextBindings);
	if (terError == condition->tag)
		return condition;
	if (IsTrue(condition))
		return EvalRecursive(args[1], contextBindings);
	return EvalRecursive(args[2], contextBindings);
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
		condition = EvalRecursive(condition, contextBindings);
		if (terError == condition->tag)
			return condition;
		if (IsTrue(condition))
			return condItemArguments ? EvalList(condItemArguments, contextBindings) : condition;
	}
	return Void();
}