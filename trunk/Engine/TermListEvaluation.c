#include "TermListEvaluation.h"
#include "TermEvaluation.h"
#include "Error.h"
#include <malloc.h>
#include <memory.h>
#include <assert.h>

static EvaluationContextBase * DoChildEvaluated(TermListEvaluationContext * evaluationContext, Term * childResult) {
	evaluationContext->currentResult = childResult;
	return THIS_CONTEXT;
}

static EvaluationContextBase * DoEvaluate(TermListEvaluationContext * evaluationContext) {
	Term * term = 0;
	if (term = IterateList(&evaluationContext->input))
		return AcquireTermEvaluationContext(THIS_CONTEXT, THIS_CONTEXT->contextBindings, term);
	if (evaluationContext->currentResult)
		THIS_CONTEXT->result = evaluationContext->currentResult;
	else
		THIS_CONTEXT->result = InvalidArgumentCount();
	return THIS_CONTEXT;
}

TermListEvaluationContext * AllocateTermListEvaluationContext() {
	TermListEvaluationContext * result = malloc(sizeof(*result));
	memset(result, 0, sizeof(*result));
	return result;
}

EvaluationContextBase * AcquireTermListEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List input) {
	TermListEvaluationContext * result = AllocateTermListEvaluationContext();
	FillEvaluationContextBase(&result->base, parent, contextBindings, (ChildEvaluatedPtr) DoChildEvaluated, (EvaluatePtr) DoEvaluate);	
	result->input = input;
	result->currentResult = 0;
	return (EvaluationContextBase *) result;
}
