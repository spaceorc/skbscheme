#include "TermEvaluation.h"
#include "Redex.h"
#include <malloc.h>
#include <memory.h>
#include <assert.h>

static EvaluationContextBase * DoChildEvaluated(TermEvaluationContext * evaluationContext, Term * childResult) {
	assert(0);
	return 0;
}

static EvaluationContextBase * DoEvaluate(TermEvaluationContext * evaluationContext) {
	switch (evaluationContext->input->tag) {
		case terRedex:
			return AcquireReductionContext(evaluationContext->base.parent, evaluationContext->base.contextBindings, evaluationContext->input->redex);
		case terSymbol:
			evaluationContext->input = Resolve(evaluationContext->base.contextBindings, evaluationContext->input->symbol);
			return (EvaluationContextBase *) evaluationContext;
		default:
			evaluationContext->base.result = evaluationContext->input;
			return (EvaluationContextBase *) evaluationContext;
	}
}

TermEvaluationContext * AllocateTermEvaluationContext() {
	TermEvaluationContext * result = malloc(sizeof(*result));
	memset(result, 0, sizeof(*result));
	return result;
}

EvaluationContextBase * AcquireTermEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, Term * input) {
	TermEvaluationContext * result = AllocateTermEvaluationContext();
	FillEvaluationContextBase(&result->base, parent, contextBindings, (ChildEvaluatedPtr) DoChildEvaluated, (EvaluatePtr) DoEvaluate);	
	result->input = input;
	return (EvaluationContextBase *) result;
}
