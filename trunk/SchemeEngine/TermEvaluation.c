#include <assert.h>

#include "TermEvaluation.h"
#include "RedexEvaluation.h"
#include "Redex.h"

static EvaluationContextBase * DoChildEvaluated(TermEvaluationContext * evaluationContext, Term * childResult) {
	assert(!"todo Add failure description here");
	return NULL;
}

static EvaluationContextBase * DoEvaluate(TermEvaluationContext * evaluationContext) {
	switch (evaluationContext->input->tag) {
		case terRedex:
			return AcquireReductionContext(THIS_CONTEXT->parent, THIS_CONTEXT->contextBindings, evaluationContext->input->redex);
		case terVariable:
			evaluationContext->input = Resolve(THIS_CONTEXT->contextBindings, evaluationContext->input->variable);
			return THIS_CONTEXT;
		default:
			THIS_CONTEXT->result = evaluationContext->input;
			return THIS_CONTEXT;
	}
}

TermEvaluationContext * AllocateTermEvaluationContext() {
	TermEvaluationContext * result = AllocateEvaluationContext(sizeof(*result));
	return result;
}

EvaluationContextBase * AcquireTermEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, Term * input) {
	TermEvaluationContext * result = AllocateTermEvaluationContext();
	FillEvaluationContextBase(&result->base, parent, contextBindings, (ChildEvaluatedPtr) DoChildEvaluated, (EvaluatePtr) DoEvaluate);	
	result->input = input;
	return (EvaluationContextBase *) result;
}
