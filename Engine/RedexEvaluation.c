#include "TermEvaluation.h"
#include "RedexEvaluation.h"
#include "Error.h"
#include <memory.h>
#include <malloc.h>
#include <assert.h>

static EvaluationContextBase * DoChildEvaluated(ReductionContext * evaluationContext, Term * childResult) {
	assert(0);
	return 0;
}

static EvaluationContextBase * DoEvaluate(ReductionContext * evaluationContext) {
	Term * function = evaluationContext->function, *term = 0;
	if (0 != function) {
		switch(function->tag) {
			case terError:
				evaluationContext->base.result = function;
				return (EvaluationContextBase *) evaluationContext;
			case terFunction:
				if (term = IterateList(&evaluationContext->iterator))
					return AcquireTermEvaluationContext((EvaluationContextBase *) evaluationContext, evaluationContext->base.contextBindings, term);
				else {
					evaluationContext->base.result = function->function(evaluationContext->arguments);
					return (EvaluationContextBase *) evaluationContext;
				}
			//case terLazyFunction:
			//	return function->lazyFunction(arguments, contextBindings);
			//case terLambda:
			//	if (!EvalArguments(&arguments, contextBindings, &error))
			//		return error;
			//	return EvalLambda(function->lambda, arguments, contextBindings);
			default:
				evaluationContext->base.result = InvalidArgumentType();
				return (EvaluationContextBase *) evaluationContext;
		}
	}
	else {
		assert(0);
		return 0;
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
	result->iterator = 0;
	result->arguments = 0;
	return (EvaluationContextBase *) result;
}
