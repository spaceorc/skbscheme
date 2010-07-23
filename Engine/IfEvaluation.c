#include <malloc.h>
#include <memory.h>

#include "IfEvaluation.h"
#include "TermEvaluation.h"
#include "Boolean.h"

static EvaluationContextBase * DoChildEvaluated(IfEvaluationContext * evaluationContext, Term * childResult) {
	Term * args[] = {0, 0}, * error = 0;
	if (TakeSeveralArguments(evaluationContext->arguments, args, &error) < 0) {
		THIS_CONTEXT->result = error;	
		return THIS_CONTEXT;
	}
	if (IsTrue(childResult))
		return AcquireTermEvaluationContext(THIS_CONTEXT->parent, THIS_CONTEXT->contextBindings, args[0]);
	return AcquireTermEvaluationContext(THIS_CONTEXT->parent, THIS_CONTEXT->contextBindings, args[1]);
}

static EvaluationContextBase * DoEvaluate(IfEvaluationContext * evaluationContext) {
	Term * condition = Iterate(&evaluationContext->arguments);
	if (!condition) {
		THIS_CONTEXT->result = InvalidArgumentCount();
		return THIS_CONTEXT;
	}
	return AcquireTermEvaluationContext(THIS_CONTEXT, THIS_CONTEXT->contextBindings, condition);
}

IfEvaluationContext * AllocateIfEvaluationContext() {
	IfEvaluationContext * result = malloc(sizeof(*result));
	memset(result, 0, sizeof(*result));
	return result;
}

EvaluationContextBase * AcquireIfEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments) {
	IfEvaluationContext * result = AllocateIfEvaluationContext();
	FillEvaluationContextBase(&result->base, parent, contextBindings, (ChildEvaluatedPtr) DoChildEvaluated, (EvaluatePtr) DoEvaluate);	
	result->arguments = arguments;
	return (EvaluationContextBase *) result;
}
