#include <memory.h>
#include <malloc.h>
#include <assert.h>

#include "TermEvaluation.h"
#include "TermListEvaluation.h"
#include "RedexEvaluation.h"
#include "Pair.h"
#include "Dictionary.h"

static EvaluationContextBase * DoChildEvaluated(ReductionContext * evaluationContext, Term * childResult) {
	if (!evaluationContext->function)
		evaluationContext->function = childResult;
	else 	
		evaluationContext->arguments = AppendList(evaluationContext->arguments, childResult);
	return THIS_CONTEXT;
}

static EvaluationContextBase * EvalLambda(ReductionContext * evaluationContext) {
	Term * formalArgument = 0, * argument = 0;
	Lambda lambda;
	List arguments = evaluationContext->arguments;
	ContextBindings * childContextBindings = 0;
	assert(terLambda == evaluationContext->function->tag);
	lambda = evaluationContext->function->lambda;
	childContextBindings = AllocateContextBindings(lambda.context);
	while(formalArgument = IterateList(&lambda.formalArguments)) {
		argument = IterateList(&arguments);
		if (!argument) {
			THIS_CONTEXT->result = InvalidArgumentCount();
			return THIS_CONTEXT;
		}
		if (terVariable != formalArgument->tag) {
			THIS_CONTEXT->result = InvalidArgumentType();
			return THIS_CONTEXT;
		}
		childContextBindings->dictionary = Set(childContextBindings->dictionary, formalArgument->variable, argument);
	}
	if (IterateList(&arguments)) {
		THIS_CONTEXT->result = InvalidArgumentCount();
		return THIS_CONTEXT;
	}
	return AcquireTermListEvaluationContext(THIS_CONTEXT->parent, childContextBindings, lambda.body);
}

static EvaluationContextBase * DoEvaluate(ReductionContext * evaluationContext) {
	Term * function = evaluationContext->function, *term = 0;
	if (!function) {
		if (term = IterateList(&evaluationContext->redex))
			return AcquireTermEvaluationContext(THIS_CONTEXT, THIS_CONTEXT->contextBindings, term);
		THIS_CONTEXT->result = InvalidArgumentCount();
		return THIS_CONTEXT;
	}
	switch(function->tag) {
		case terFunction:
			if (term = IterateList(&evaluationContext->redex))
				return AcquireTermEvaluationContext(THIS_CONTEXT, THIS_CONTEXT->contextBindings, term);
			THIS_CONTEXT->result = function->function(evaluationContext->arguments);
			return THIS_CONTEXT;
		case terLazyFunction:
			return function->lazy.acquireEvaluationContext(THIS_CONTEXT->parent, THIS_CONTEXT->contextBindings, evaluationContext->redex);
		case terLambda:
			if (term = IterateList(&evaluationContext->redex))
				return AcquireTermEvaluationContext(THIS_CONTEXT, THIS_CONTEXT->contextBindings, term);
			return EvalLambda(evaluationContext);
		default:
			THIS_CONTEXT->result = InvalidArgumentType();
			return THIS_CONTEXT;
	}
}

ReductionContext * AllocateReductionContext() {
	ReductionContext * result = malloc(sizeof(*result));
	memset(result, 0, sizeof(*result));
	return result;
}

EvaluationContextBase * AcquireReductionContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List redex) {
	ReductionContext * result = AllocateReductionContext();
	FillEvaluationContextBase(&result->base, parent, contextBindings, (ChildEvaluatedPtr) DoChildEvaluated, (EvaluatePtr) DoEvaluate);	
	result->redex = redex;
	result->function = 0;
	result->arguments = 0;
	return (EvaluationContextBase *) result;
}
