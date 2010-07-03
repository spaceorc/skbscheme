#include "BooleanEvaluation.h"
#include "Error.h"
#include <malloc.h>
#include <memory.h>

static EvaluationContextBase * DoChildEvaluated(EvaluationContextBase * evaluationContext, Term * childResult) {
	THIS_CONTEXT->result = InvalidArgumentCount();
	return THIS_CONTEXT;
}

static EvaluationContextBase * DoEvaluate(EvaluationContextBase * evaluationContext) {
	THIS_CONTEXT->result = InvalidArgumentCount();
	return THIS_CONTEXT;
}

EvaluationContextBase * AllocateEmptyLazyEvaluationContext() {
	EvaluationContextBase * result = malloc(sizeof(*result));
	memset(result, 0, sizeof(*result));
	return result;
}

EvaluationContextBase * AcquireEmptyLazyEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments) {
	EvaluationContextBase * result = AllocateEmptyLazyEvaluationContext();
	FillEvaluationContextBase(result, parent, contextBindings, (ChildEvaluatedPtr) DoChildEvaluated, (EvaluatePtr) DoEvaluate);	
	return (EvaluationContextBase *) result;
}
