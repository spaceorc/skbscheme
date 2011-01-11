#include <memory.h>

#include "Evaluation.h"
#include "TermEvaluation.h"
#include "Library.h"

void FillEvaluationContextBase(EvaluationContextBase * base, EvaluationContextBase * parent, ContextBindings * contextBindings, ChildEvaluatedPtr childEvaluated, EvaluatePtr evaluate) {
	memset(base, 0, sizeof(*base));
	base->childEvaluated = childEvaluated;
	base->evaluate = evaluate;
	base->parent = parent;
	base->contextBindings = contextBindings;
}

Term * Evaluate(EvaluationContextBase * evaluationContext) {
	while(1) {
		if (!evaluationContext->result)
			evaluationContext = evaluationContext->evaluate(evaluationContext);
		else if (!evaluationContext->parent || terError == evaluationContext->result->tag)
			return evaluationContext->result;
		else
			evaluationContext = evaluationContext->parent->childEvaluated(evaluationContext->parent, evaluationContext->result);
	}
}

Term * EvalIterative(Term * term, ContextBindings * contextBindings) {
	return Evaluate(AcquireTermEvaluationContext(NULL, contextBindings, term));
}