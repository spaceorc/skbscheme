#include <malloc.h>
#include <memory.h>

#include "CondEvaluation.h"
#include "TermEvaluation.h"
#include "TermListEvaluation.h"
#include "Boolean.h"

static EvaluationContextBase * DoChildEvaluated(CondEvaluationContext * evaluationContext, Term * childResult) {
	if (IsTrue(childResult)) {
		if (evaluationContext->condItemArguments)
			return AcquireTermListEvaluationContext(THIS_CONTEXT->parent, THIS_CONTEXT->contextBindings, evaluationContext->condItemArguments);
		
		THIS_CONTEXT->result = childResult;
	}
	return THIS_CONTEXT;
}

static EvaluationContextBase * DoEvaluate(CondEvaluationContext * evaluationContext) {
	Term * condItem = 0, * condition = 0;
	List condItemArguments = 0;
	condItem = IterateList(&evaluationContext->arguments);
	if (!condItem) {
		THIS_CONTEXT->result = Void();
		return THIS_CONTEXT;
	}
	if (terRedex != condItem->tag) {
		THIS_CONTEXT->result = BadSyntax();
		return THIS_CONTEXT;
	}
	condItemArguments = condItem->redex;
	condition = IterateList(&condItemArguments);
	if (!condition) {
		THIS_CONTEXT->result = BadSyntax();
		return THIS_CONTEXT;
	}
	evaluationContext->condItemArguments = condItemArguments;
	return AcquireTermEvaluationContext(THIS_CONTEXT, THIS_CONTEXT->contextBindings, condition);
}

CondEvaluationContext * AllocateCondEvaluationContext() {
	CondEvaluationContext * result = malloc(sizeof(*result));
	memset(result, 0, sizeof(*result));
	return result;
}

EvaluationContextBase * AcquireCondEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments) {
	CondEvaluationContext * result = AllocateCondEvaluationContext();
	FillEvaluationContextBase(&result->base, parent, contextBindings, (ChildEvaluatedPtr) DoChildEvaluated, (EvaluatePtr) DoEvaluate);	
	result->arguments = arguments;
	return (EvaluationContextBase *) result;
}
