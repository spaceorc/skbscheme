#include <assert.h>

#include "Definitions.h"
#include "Dictionary.h"
#include "Memory.h"

Term * LazyFunctionLet(List arguments, ContextBindings * contextBindings, int inExpressionMode) {
	Term * args[] = {0, 0}, * error = 0, * current = 0, * let[] = {0, 0};
	List lets = 0;
	ContextBindings * childContextBindings = AllocateContextBindings(contextBindings);
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (terRedex != args[0]->tag)
		return InvalidArgumentType();
	lets = args[0]->redex;
	while (current = IterateList(&lets)) {
		Term * value = 0;
		if (terRedex != current->tag)
			return InvalidArgumentType();
		if (TakeSeveralArguments(current->redex, let, &error) < 0)
			return error;
		if (terSymbol != let[0]->tag)
			return InvalidArgumentType();
		value = Eval(let[1], contextBindings);
		if (value->tag == terError)
			return value;
		childContextBindings->dictionary = InternalSet(childContextBindings->dictionary, let[0]->symbol, value);
	}

	return Eval(args[1], childContextBindings);
}

Term * InternalDefineFunction(List definition, Term * first, List arguments, ContextBindings * contextBindings, int inExpressionMode) {
	Term * name = IterateList(&definition);
	Term * second = 0;
	ContextBindings * childContextBindings = AllocateContextBindings(contextBindings);
	if (terSymbol != name->tag)
		return InvalidArgumentType();
	while (second = IterateList(&arguments)) {
		/* :-) plt scheme ignores, that Empty() eq? */EvalInDefineMode(first, childContextBindings);
		first = second;
	}
	contextBindings->dictionary = InternalSet(contextBindings->dictionary, name->symbol, DefineFunction(definition, first, childContextBindings, !inExpressionMode));
	return Empty();
}

Term * LazyFunctionDefine(List arguments, ContextBindings * contextBindings, int inExpressionMode) {
	Term * prototype = IterateList(&arguments);
	Term * body = IterateList(&arguments);
	if (0 == body)
		return InvalidArgumentCount();
	switch(prototype->tag) {
		case terSymbol:
			contextBindings->dictionary = InternalSet(contextBindings->dictionary, prototype->symbol, Eval(body, contextBindings));
			return Empty();
		case terRedex:
			return InternalDefineFunction(prototype->redex, body, arguments, contextBindings, inExpressionMode);
		case terError:
			return prototype;
		default:
			return InvalidArgumentType();
	}
}

Term * LazyFunctionLambda(List arguments, ContextBindings * contextBindings, int inExpressionMode) {
	Term * args[] = {0, 0}, * error = 0;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (terRedex == args[0]->tag)
		return DefineFunction(args[0]->redex, args[1], contextBindings, 1);
	else
		return InvalidArgumentType();
	return Empty();
}