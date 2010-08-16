#include <malloc.h>
#include <memory.h>
#include <assert.h>

#include "List.h"
#include "Constructors.h"
#include "DefineEvaluation.h"
#include "TermEvaluation.h"
#include "Dictionary.h"

static EvaluationContextBase * DefineChildEvaluated(DefineEvaluationContext * evaluationContext, Term * childResult) {
	THIS_CONTEXT->contextBindings->dictionary = Set(THIS_CONTEXT->contextBindings->dictionary, evaluationContext->name, childResult);
	THIS_CONTEXT->result = Void();
	return THIS_CONTEXT;
}

static EvaluationContextBase * DefineLambdaChildEvaluated(DefineLambdaEvaluationContext * evaluationContext, Term * childResult) {
	assert(0);
	return 0;
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

static EvaluationContextBase * DefineLambdaEvaluate(DefineLambdaEvaluationContext * evaluationContext) {
	Term * prototype = 0;
	List arguments = evaluationContext->arguments;
	if (!(prototype = Iterate(&arguments))) {
		THIS_CONTEXT->result = InvalidArgumentCount();
		return THIS_CONTEXT;
	}
	if (!arguments) {
		THIS_CONTEXT->result = InvalidArgumentCount(); // todo ??? plt says this: "lambda: bad syntax"
		return THIS_CONTEXT;
	} 
	if (terRedex != prototype->tag) {
		THIS_CONTEXT->result = InvalidArgumentType();
		return THIS_CONTEXT;
	}
	THIS_CONTEXT->result = MakeLambda(prototype->redex, arguments, THIS_CONTEXT->contextBindings);
	return THIS_CONTEXT;
}

static EvaluationContextBase * DefineEvaluate(DefineEvaluationContext * evaluationContext) {
	Term * prototype = 0, * value = 0;
	List arguments = evaluationContext->arguments;
	if (!(prototype = Iterate(&arguments))) {
		THIS_CONTEXT->result = InvalidArgumentCount();
		return THIS_CONTEXT;
	}
	switch(prototype->tag) {
		case terVariable:
			value = Iterate(&arguments);
			if (!value) {
				THIS_CONTEXT->result = InvalidArgumentCount(); // todo ??? plt says this: "define: bad syntax (missing expression after identifier)"
				return THIS_CONTEXT;
			} 
			if (Iterate(&arguments)) {
				THIS_CONTEXT->result = InvalidArgumentCount(); // todo ??? plt says this: "define: bad syntax (multiple expressions after identifier)"
				return THIS_CONTEXT;
			}
			evaluationContext->name = prototype->variable;
			return AcquireTermEvaluationContext(THIS_CONTEXT, THIS_CONTEXT->contextBindings, value);
		case terRedex:
			THIS_CONTEXT->result = InternalDefineFunction(prototype->redex, arguments, THIS_CONTEXT->contextBindings);
			return THIS_CONTEXT;
		default: 
			THIS_CONTEXT->result = InvalidArgumentType();
			return THIS_CONTEXT;
	}
}

DefineEvaluationContext * AllocateDefineEvaluationContext() {
	DefineEvaluationContext * result = malloc(sizeof(*result));
	memset(result, 0, sizeof(*result));
	return result;
}

DefineLambdaEvaluationContext * AllocateDefineLambdaEvaluationContext() {
	DefineLambdaEvaluationContext * result = malloc(sizeof(*result));
	memset(result, 0, sizeof(*result));
	return result;
}

EvaluationContextBase * AcquireDefineEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments) {
	DefineEvaluationContext * result = AllocateDefineEvaluationContext();
	FillEvaluationContextBase(&result->base, parent, contextBindings, (ChildEvaluatedPtr) DefineChildEvaluated, (EvaluatePtr) DefineEvaluate);	
	result->arguments = arguments;
	return (EvaluationContextBase *) result;
}

EvaluationContextBase * AcquireDefineLambdaEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments) {
	DefineLambdaEvaluationContext * result = AllocateDefineLambdaEvaluationContext();
	FillEvaluationContextBase(&result->base, parent, contextBindings, (ChildEvaluatedPtr) DefineLambdaChildEvaluated, (EvaluatePtr) DefineLambdaEvaluate);	
	result->arguments = arguments;
	return (EvaluationContextBase *) result;
}