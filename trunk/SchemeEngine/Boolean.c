#include "Boolean.h"
#include "List.h"
#include "Void.h"
#include "MemoryManager.h"
#include "Redex.h"

Term * True() {
	Term * result = AllocateTerm(terBoolean);
	result->boolean = TRUE;
	return result;
}

Term * False() {
	Term * result = AllocateTerm(terBoolean);
	result->boolean = FALSE;
	return result;
}

int IsTrue(Term * term) {
	return !IsFalse(term);
}

int IsFalse(Term * term) {
	return term->tag == terBoolean && !term->boolean;
}

Term * LazyFunctionAnd(List arguments, ContextBindings * contextBindings) {
	Term * arg = NULL, * term = NULL;
	while (arg = Iterate(&arguments)) {
		term = EvalRecursive(arg, contextBindings);
		if (terError == term->tag || IsFalse(term))
			return term;
	}
	if (term)
		return term;
	return True();
}

Term * LazyFunctionOr(List arguments, ContextBindings * contextBindings) {
	Term * arg = NULL, * term = NULL;
	while (arg = Iterate(&arguments)) {
		term = EvalRecursive(arg, contextBindings);
		if (terError == term->tag || IsTrue(term))
			return term;
	}
	if (term)
		return term;
	return False();
}

Term * LazyFunctionIf(List arguments, ContextBindings * contextBindings) {
	Term * args[] = {NULL, NULL, NULL}, * error = NULL, * condition = NULL;
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
	Term * condItem = NULL, * condition = NULL;
	List condItemArguments = NULL;
	while (condItem = Iterate(&arguments)) {
		if (terRedex != condItem->tag)
			return BadSyntax();
		condItemArguments = condItem->redex;
		condition = Iterate(&condItemArguments);
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