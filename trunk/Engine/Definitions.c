#include <assert.h>

#include "Definitions.h"
#include "Dictionary.h"
#include "Memory.h"

Term * LazyFunctionLet(List arguments, ContextBindings * contextBindings) {
	ContextBindings * childContextBindings = 0;
	Term * lets = 0, * current = 0;
	List letsList = 0;
	if (!(lets = IterateList(&arguments)))
		return InvalidArgumentCount();
	if (terRedex != lets->tag)
		return InvalidArgumentType();
	letsList = lets->redex;
	childContextBindings = AllocateContextBindings(contextBindings);
	while (current = IterateList(&letsList)) {
		Term * value = 0, * let[] = {0, 0}, * error;
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
	return EvalList(arguments, childContextBindings);
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