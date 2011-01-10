#include <assert.h>

#include "Definitions.h"
#include "Dictionary.h"
#include "List.h"
#include "Void.h"
#include "MemoryManager.h"

Term * LazyFunctionLet(List arguments, ContextBindings * contextBindings) {
	ContextBindings * childContextBindings = 0;
	Term * lets = 0, * current = 0;
	List letsList = 0;
	if (!(lets = Iterate(&arguments)))
		return InvalidArgumentCount();
	CheckTermType(lets, terRedex);
	letsList = lets->redex;
	childContextBindings = AllocateContextBindings(contextBindings);
	while (current = Iterate(&letsList)) {
		Term * value = 0, * let[] = {0, 0}, * error;
		CheckTermType(current, terRedex);
		if (TakeSeveralArguments(current->redex, let, &error) < 0)
			return error;
		CheckTermType(let[0], terVariable);
		EvalTermAndCheckError(value, let[1], contextBindings);
		childContextBindings->dictionary = Set(childContextBindings->dictionary, let[0]->variable, value);
	}
	return EvalList(arguments, childContextBindings);
}

static Term * InternalDefineFunction(List definition, List body, ContextBindings * contextBindings) {
	Term * name = Iterate(&definition);
	Term * second = 0;
	if (!body)
		return InvalidArgumentCount(); // todo ??? plt says this: "define: bad syntax (no expressions for procedure body)"
	CheckTermType(name, terVariable);
	contextBindings->dictionary = Set(contextBindings->dictionary, name->variable, MakeLambda(definition, body, contextBindings));
	return Void();
}

Term * LazyFunctionDefine(List arguments, ContextBindings * contextBindings) {
	Term * prototype = 0, * value = 0;
	if (!(prototype = Iterate(&arguments)))
		return InvalidArgumentCount();
	switch(prototype->tag) {
		case terVariable:
			value = Iterate(&arguments);
			if (!value)
				return InvalidArgumentCount(); // todo ??? plt says this: "define: bad syntax (missing expression after identifier)"
			if (Iterate(&arguments))
				return InvalidArgumentCount(); // todo ??? plt says this: "define: bad syntax (multiple expressions after identifier)"
			EvalTermAndCheckError(value, value, contextBindings);
			contextBindings->dictionary = Set(contextBindings->dictionary, prototype->variable, value);
			return Void();
		case terRedex:
			return InternalDefineFunction(prototype->redex, arguments, contextBindings);
		case terError:
			return prototype;
		default:
			return InvalidArgumentType();
	}
}

Term * LazyFunctionLambda(List arguments, ContextBindings * contextBindings) {
	Term * prototype = Iterate(&arguments);
	if (!prototype)
		return InvalidArgumentCount();
	if (!arguments)
		return InvalidArgumentCount(); // todo ??? plt says this: "lambda: bad syntax"
	CheckErrorTerm(prototype);
	CheckTermType(prototype, terRedex);
	return MakeLambda(prototype->redex, arguments, contextBindings);
}