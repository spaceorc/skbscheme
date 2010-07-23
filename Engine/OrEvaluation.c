#include <malloc.h>
#include <memory.h>

#include "OrEvaluation.h"
#include "TermEvaluation.h"
#include "Boolean.h"

static EvaluationContextBase * DoChildEvaluated(OrEvaluationContext * evaluationContext, Term * childResult) {
	if (IsTrue(childResult))
		THIS_CONTEXT->result = childResult;
	return THIS_CONTEXT;
}

static EvaluationContextBase * DoEvaluate(OrEvaluationContext * evaluationContext) {
	Term * arg = 0;	
	arg = Iterate(&evaluationContext->arguments);
	if (!arg) {
		THIS_CONTEXT->result = False();
		return THIS_CONTEXT;
	}
	return AcquireTermEvaluationContext(THIS_CONTEXT, THIS_CONTEXT->contextBindings, arg);
}

OrEvaluationContext * AllocateOrEvaluationContext() {
	OrEvaluationContext * result = malloc(sizeof(*result));
	memset(result, 0, sizeof(*result));
	return result;
}

EvaluationContextBase * AcquireOrEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments) {
	OrEvaluationContext * result = AllocateOrEvaluationContext();
	FillEvaluationContextBase(&result->base, parent, contextBindings, (ChildEvaluatedPtr) DoChildEvaluated, (EvaluatePtr) DoEvaluate);	
	result->arguments = arguments;
	return (EvaluationContextBase *) result;
}