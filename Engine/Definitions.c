#include <assert.h>

#include "Definitions.h"
#include "Dictionary.h"
#include "MemoryManager.h"
#include "Error.h"

Term * LazyFunctionLet(List arguments, ContextBindings * contextBindings) {
	ContextBindings * childContextBindings = 0;
	Term * lets = 0, * current = 0;
	List letsList = 0;
	if (!(lets = IterateList(&arguments)))
		return InvalidArgumentCount();
	CheckTermType(lets, terRedex);
	letsList = lets->redex;
	childContextBindings = AllocateContextBindings(contextBindings);
	while (current = IterateList(&letsList)) {
		Term * value = 0, * let[] = {0, 0}, * error;
		CheckTermType(current, terRedex);
		if (TakeSeveralArguments(current->redex, let, &error) < 0)
			return error;
		CheckTermType(let[0], terSymbol);
		EvalTermAndCheckError(value, let[1], contextBindings);
		childContextBindings->dictionary = InternalSet(childContextBindings->dictionary, let[0]->symbol, value);
	}
	return EvalList(arguments, childContextBindings);
}

Term * InternalDefineFunction(List definition, List body, ContextBindings * contextBindings) {
	Term * name = IterateList(&definition);
	Term * second = 0;
	if (!body)
		return InvalidArgumentCount(); // todo ??? plt says this: "define: bad syntax (no expressions for procedure body)"
	CheckTermType(name, terSymbol);
	contextBindings->dictionary = InternalSet(contextBindings->dictionary, name->symbol, MakeLambda(definition, body, contextBindings));
	return Empty();
}

Term * LazyFunctionDefine(List arguments, ContextBindings * contextBindings) {
	Term * prototype = 0, * value = 0;
	if (!(prototype = IterateList(&arguments)))
		return InvalidArgumentCount();
	switch(prototype->tag) {
		case terSymbol:
			value = IterateList(&arguments);
			if (!value)
				return InvalidArgumentCount(); // todo ??? plt says this: "define: bad syntax (missing expression after identifier)"
			if (IterateList(&arguments))
				return InvalidArgumentCount(); // todo ??? plt says this: "define: bad syntax (multiple expressions after identifier)"
			EvalTermAndCheckError(value, value, contextBindings);
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
	if (!arguments)
		return InvalidArgumentCount(); // todo ??? plt says this: "lambda: bad syntax"
	CheckErrorTerm(prototype);
	CheckTermType(prototype, terRedex);
	return MakeLambda(prototype->redex, arguments, contextBindings);
}