#include <assert.h>

#include "Definitions.h"
#include "Dictionary.h"
#include "Memory.h"

Term * LazyFunctionLet(List arguments, ContextBindings * contextBindings) {
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

Term * InternalDefineFunction(List definition, List body, ContextBindings * contextBindings) {
	Term * name = IterateList(&definition);
	Term * second = 0;
	if (!body)
		return InvalidArgumentCount(); // todo ??? plt says this: "define: bad syntax (no expressions for procedure body)"
	if (terSymbol != name->tag)
		return InvalidArgumentType();
	contextBindings->dictionary = InternalSet(contextBindings->dictionary, name->symbol, DefineFunction(definition, body, contextBindings));
	return Empty();
}

Term * LazyFunctionDefine(List arguments, ContextBindings * contextBindings) {
	Term * prototype = IterateList(&arguments), * value = 0;
	switch(prototype->tag) {
		case terSymbol:
			value = IterateList(&arguments);
			if (!value)
				return InvalidArgumentCount(); // todo ??? plt says this: "define: bad syntax (missing expression after identifier)"
			if (IterateList(&arguments))
				return InvalidArgumentCount(); // todo ??? plt says this: "define: bad syntax (multiple expressions after identifier)"
			value = Eval(value, contextBindings);
			if (terError == value->tag)
				return value;
			contextBindings->dictionary = InternalSet(contextBindings->dictionary, prototype->symbol, value);
			return Empty();
		case terRedex:
			return InternalDefineFunction(prototype->redex, arguments, contextBindings);
		case terError:
			return prototype;
		default:
			return InvalidArgumentType();
	}
}

Term * LazyFunctionLambda(List arguments, ContextBindings * contextBindings) {
	Term * prototype = IterateList(&arguments);
	if (!prototype)
		return InvalidArgumentCount();
	if (terError == prototype->tag)
		return prototype;
	if (terRedex != prototype->tag)
		return InvalidArgumentType();
	if (!arguments)
		return InvalidArgumentCount(); // todo ??? plt says this: "lambda: bad syntax"
	return DefineFunction(prototype->redex, arguments, contextBindings);
}