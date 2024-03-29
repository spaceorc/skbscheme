#include "List.h"
#include "AndEvaluation.h"
#include "TermEvaluation.h"
#include "Boolean.h"

static EvaluationContextBase * DoChildEvaluated(AndEvaluationContext * evaluationContext, Term * childResult) {
	if (IsFalse(childResult))
		THIS_CONTEXT->result = childResult;
	else
		evaluationContext->currentResult = childResult;
	return THIS_CONTEXT;
}

static EvaluationContextBase * DoEvaluate(AndEvaluationContext * evaluationContext) {
	Term * arg = NULL;
	arg = Iterate(&evaluationContext->arguments);
	if (!arg) {
		THIS_CONTEXT->result = evaluationContext->currentResult;
		return THIS_CONTEXT;
	}
	return AcquireTermEvaluationContext(THIS_CONTEXT, THIS_CONTEXT->contextBindings, arg);
}

AndEvaluationContext * AllocateAndEvaluationContext() {
	AndEvaluationContext * result = AllocateEvaluationContext(sizeof(*result));
	return result;
}

EvaluationContextBase * AcquireAndEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments) {
	AndEvaluationContext * result = AllocateAndEvaluationContext();
	FillEvaluationContextBase(&result->base, parent, contextBindings, (ChildEvaluatedPtr) DoChildEvaluated, (EvaluatePtr) DoEvaluate);	
	result->arguments = arguments;
	result->currentResult = True();
	return (EvaluationContextBase *) result;
}