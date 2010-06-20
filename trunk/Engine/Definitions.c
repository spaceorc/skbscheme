#include <assert.h>
#include "Definitions.h"
#include "Dictionary.h"

Term * LazyFunctionLet(List arguments, ContextBindings * contextBindings) {
	Term * args[] = {0, 0}, * error = 0, * current = 0, * let[] = {0, 0};
	List lets = 0;
	ContextBindings * childContextBindings = AllocateContextBindings(contextBindings);
    // todo free context bindings
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	assert(args[0]->tag == terRedex); // todo InvalidArgumentType()
	lets = args[0]->redex;
	while (current = IterateList(&lets)) {
		assert(current->tag == terRedex); // todo InvalidArgumentType()
		if (TakeSeveralArguments(current->redex, let, &error) < 0)
			return error;
		assert(let[0]->tag == terSymbol); // todo InvalidArgumentType()
		childContextBindings->dictionary = InternalSetFromLimited(childContextBindings->dictionary, let[0]->symbol, let[1]);
	}
	
	return Eval(args[1], childContextBindings);
}

Term * LazyFunctionDefine(List arguments, ContextBindings * contextBindings) {
	Term * args[] = {0, 0}, * error = 0;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (terSymbol == args[0]->tag)
		contextBindings->dictionary = InternalSetFromLimited(contextBindings->dictionary, args[0]->symbol, Eval(args[1], contextBindings));
	else {
		List definition = 0;
		Term * name;
		assert(terRedex == args[0]->tag); // todo maybe error : InvalidDefine ???
		definition = args[0]->redex;
		name = IterateList(&definition);
		assert(terSymbol == name->tag); // todo maybe error : InvalidDefine ???
		contextBindings->dictionary = InternalSetFromLimited(contextBindings->dictionary, name->symbol, DefineFunction(definition, args[1]));
	}
	return Nil();
}

Term * DefinedFunctionApply(DefinedFunction definedFunction, List arguments, ContextBindings * contextBindings) {
	Term * formalArgument = 0, * argument = 0;
	ContextBindings * childContextBindings = AllocateContextBindings(contextBindings);
    // todo free context bindings
	while(formalArgument = IterateList(&definedFunction.formalArguments)) {
		argument = IterateList(&arguments);
		assert(argument); // todo - error: InvalidArgumentCount();
		assert(formalArgument->tag == terSymbol); // todo - maybe error: InvalidDefine ??;
		childContextBindings->dictionary = InternalSetFromLimited(childContextBindings->dictionary, formalArgument->symbol, argument);
	}
	assert(!IterateList(&arguments)); // todo - error: InvalidArgumentCount();
	return Eval(definedFunction.function, childContextBindings);
}
