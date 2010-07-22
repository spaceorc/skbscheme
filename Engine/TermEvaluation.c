#include <malloc.h>
#include <memory.h>
#include <assert.h>

#include "TermEvaluation.h"
#include "RedexEvaluation.h"
#include "Redex.h"

static EvaluationContextBase * DoChildEvaluated(TermEvaluationContext * evaluationContext, Term * childResult) {
	assert(0);
	return 0;
}

static EvaluationContextBase * DoEvaluate(TermEvaluationContext * evaluationContext) {
	switch (evaluationContext->input->tag) {
		case terRedex:
			return AcquireReductionContext(THIS_CONTEXT->parent, THIS_CONTEXT->contextBindings, evaluationContext->input->redex);
		case terSymbol:
			evaluationContext->input = Resolve(THIS_CONTEXT->contextBindings, evaluationContext->input->symbol);
			return THIS_CONTEXT;
		default:
			THIS_CONTEXT->result = evaluationContext->input;
			return THIS_CONTEXT;
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
