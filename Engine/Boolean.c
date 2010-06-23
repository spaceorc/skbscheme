#include "Boolean.h"
#include "Memory.h"
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
	Term * args[] = {0, 0}, * error = 0;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (IsTrue(Eval(args[0], contextBindings)))
		return Eval(args[1], contextBindings);
	return False();
}

Term * LazyFunctionOr(List arguments, ContextBindings * contextBindings) {
	Term * args[] = {0, 0}, * error = 0;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (IsFalse(Eval(args[0], contextBindings)))
		return Eval(args[1], contextBindings);
	return True();
}

Term * LazyFunctionIf(List arguments, ContextBindings * contextBindings) {
	Term * args[] = {0, 0, 0}, * error = 0;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (IsTrue(Eval(args[0], contextBindings)))
		return Eval(args[1], contextBindings);
	return Eval(args[2], contextBindings);
}