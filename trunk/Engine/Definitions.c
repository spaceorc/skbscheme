#include "Definitions.h"

Term * LazyFunctionLet(List arguments, ContextBindings * contextBindings) {
	Term * args[] = {0, 0}, * error = 0, * current = 0, * let[] = {0, 0};
	List lets = 0;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (args[0]->tag != terRedex)
		return InvalidArgumentType();
	lets = args[0]->redex;
	while (current = IterateList(&lets)) {
		if (current->tag != terRedex)
			return InvalidArgumentType();
		if (TakeSeveralArguments(current->redex, let, &error) < 0)
			return error;
		if (let[0]->tag != terSymbol)
			return InvalidArgumentType();
		InternalSetFromLimited(contextBindings->dictionary, let[0]->symbol, let[1]);
	}
	
	return Eval(args[1], contextBindings);
}

Term * LazyFunctionDefine(List arguments, ContextBindings * contextBindings) {
	return Nil();
}