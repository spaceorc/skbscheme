#include <assert.h>

#include "Definitions.h"
#include "Dictionary.h"
#include "Memory.h"

Term * LazyFunctionLet(List arguments, ContextBindings * contextBindings) {
	Term * args[] = {0, 0}, * error = 0, * current = 0, * let[] = {0, 0};
	List lets = 0;
	ContextBindings * childContextBindings = AllocateContextBindings(contextBindings);
    // todo free context bindings
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

Term * LazyFunctionDefine(List arguments, ContextBindings * contextBindings) {
	Term * args[] = {0, 0}, * error = 0;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (terSymbol == args[0]->tag)
		contextBindings->dictionary = InternalSet(contextBindings->dictionary, args[0]->symbol, Eval(args[1], contextBindings));
	else if (terRedex == args[0]->tag) {
		List definition = 0;
		Term * name;
		definition = args[0]->redex;
		name = IterateList(&definition);
		if (terSymbol != name->tag)
			return InvalidArgumentType();
		contextBindings->dictionary = InternalSet(contextBindings->dictionary, name->symbol, DefineFunction(definition, args[1]));
	}
	else
		return InvalidArgumentType();
	return Nil();
}

Term * DefinedFunctionApply(DefinedFunction definedFunction, List arguments, ContextBindings * contextBindings) {
	Term * formalArgument = 0, * argument = 0;
	ContextBindings * childContextBindings = AllocateContextBindings(contextBindings);
    // todo free context bindings
	while(formalArgument = IterateList(&definedFunction.formalArguments)) {
		argument = IterateList(&arguments);
		if (!argument)
			return InvalidArgumentCount();
		if (formalArgument->tag != terSymbol)
			return InvalidArgumentType();
		childContextBindings->dictionary = InternalSet(childContextBindings->dictionary, formalArgument->symbol, argument);
	}
	if (IterateList(&arguments))
		return InvalidArgumentCount();
	return Eval(definedFunction.function, childContextBindings);
}
