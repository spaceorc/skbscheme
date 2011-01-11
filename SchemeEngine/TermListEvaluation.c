#include <assert.h>

#include "List.h"
#include "TermListEvaluation.h"
#include "TermEvaluation.h"

static EvaluationContextBase * DoChildEvaluated(TermListEvaluationContext * evaluationContext, Term * childResult) {
	evaluationContext->currentResult = childResult;
	return THIS_CONTEXT;
}

static EvaluationContextBase * DoEvaluate(TermListEvaluationContext * evaluationContext) {
	Term * term = NULL;
	if (term = Iterate(&evaluationContext->input))
		return AcquireTermEvaluationContext(THIS_CONTEXT, THIS_CONTEXT->contextBindings, term);
	if (evaluationContext->currentResult)
		THIS_CONTEXT->result = evaluationContext->currentResult;
	else
		THIS_CONTEXT->result = InvalidArgumentCount();
	return THIS_CONTEXT;
}

TermListEvaluationContext * AllocateTermListEvaluationContext() {
	TermListEvaluationContext * result = AllocateEvaluationContext(sizeof(*result));
	return result;
}

EvaluationContextBase * AcquireTermListEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List input) {
	TermListEvaluationContext * result = AllocateTermListEvaluationContext();
	FillEvaluationContextBase(&result->base, parent, contextBindings, (ChildEvaluatedPtr) DoChildEvaluated, (EvaluatePtr) DoEvaluate);	
	result->input = input;
	result->currentResult = NULL;
	return (EvaluationContextBase *) result;
}
