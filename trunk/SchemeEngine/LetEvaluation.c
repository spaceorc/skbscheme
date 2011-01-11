#include <assert.h>

#include "List.h"
#include "LetEvaluation.h"
#include "TermEvaluation.h"
#include "TermListEvaluation.h"
#include "Dictionary.h"

static EvaluationContextBase * DoChildEvaluated(LetEvaluationContext * evaluationContext, Term * childResult) {
	evaluationContext->childContextBindings->dictionary = Set(evaluationContext->childContextBindings->dictionary, evaluationContext->currentLetVariable, childResult);
	return THIS_CONTEXT;
}

static EvaluationContextBase * DoEvaluate(LetEvaluationContext * evaluationContext) {
	Term * lets = NULL, * current = NULL, * let[] = {NULL, NULL}, * error = NULL;
	if (!evaluationContext->childContextBindings) {
		if (!(lets = Iterate(&evaluationContext->arguments))) {
			THIS_CONTEXT->result = InvalidArgumentCount();
			return THIS_CONTEXT;
		}
		if (terRedex != lets->tag) {
			THIS_CONTEXT->result = InvalidArgumentType();
			return THIS_CONTEXT;
		}
		evaluationContext->letsList = lets->redex;
		evaluationContext->childContextBindings = AllocateContextBindings(THIS_CONTEXT->contextBindings);
	}
	if (!(current = Iterate(&evaluationContext->letsList)))
		return AcquireTermListEvaluationContext(THIS_CONTEXT->parent, evaluationContext->childContextBindings, evaluationContext->arguments);

	if (terRedex != current->tag) {
		THIS_CONTEXT->result = InvalidArgumentType();
		return THIS_CONTEXT;
	}
	if (TakeSeveralArguments(current->redex, let, &error) < 0) {
		THIS_CONTEXT->result = error;
		return THIS_CONTEXT;
	}
	if (terVariable != let[0]->tag) {
		THIS_CONTEXT->result = InvalidArgumentType();
		return THIS_CONTEXT;
	}
	
	evaluationContext->currentLetVariable = let[0]->variable;
	return AcquireTermEvaluationContext(THIS_CONTEXT, THIS_CONTEXT->contextBindings, let[1]);
}

LetEvaluationContext * AllocateLetEvaluationContext() {
	LetEvaluationContext * result = AllocateEvaluationContext(sizeof(*result));
	return result;
}

EvaluationContextBase * AcquireLetEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments) {
	LetEvaluationContext * result = AllocateLetEvaluationContext();
	FillEvaluationContextBase(&result->base, parent, contextBindings, (ChildEvaluatedPtr) DoChildEvaluated, (EvaluatePtr) DoEvaluate);	
	result->arguments = arguments;
	result->childContextBindings = NULL;
	return (EvaluationContextBase *) result;
}
