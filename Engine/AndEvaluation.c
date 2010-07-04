#include "AndEvaluation.h"
#include "TermEvaluation.h"
#include "Boolean.h"
#include "Error.h"
#include <malloc.h>
#include <memory.h>

static EvaluationContextBase * DoChildEvaluated(AndEvaluationContext * evaluationContext, Term * childResult) {
	if (IsFalse(childResult))
		THIS_CONTEXT->result = childResult;
	else
		evaluationContext->currentResult = childResult;
	return THIS_CONTEXT;
}

static EvaluationContextBase * DoEvaluate(AndEvaluationContext * evaluationContext) {
	Term * arg = 0;	
	arg = IterateList(&evaluationContext->arguments);
	if (!arg) {
		THIS_CONTEXT->result = evaluationContext->currentResult;
		return THIS_CONTEXT;
	}
	return AcquireTermEvaluationContext(THIS_CONTEXT, THIS_CONTEXT->contextBindings, arg);
}

AndEvaluationContext * AllocateAndEvaluationContext() {
	AndEvaluationContext * result = malloc(sizeof(*result));
	memset(result, 0, sizeof(*result));
	return result;
}

EvaluationContextBase * AcquireAndEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments) {
	AndEvaluationContext * result = AllocateAndEvaluationContext();
	FillEvaluationContextBase(&result->base, parent, contextBindings, (ChildEvaluatedPtr) DoChildEvaluated, (EvaluatePtr) DoEvaluate);	
	result->arguments = arguments;
	result->currentResult = True();
	return (EvaluationContextBase *) result;
}