#include "Evaluation.h"
#include "TermEvaluation.h"
#include "Library.h"
#include <memory.h>

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
		else if (evaluationContext->parent)
			evaluationContext = evaluationContext->parent->childEvaluated(evaluationContext->parent, evaluationContext->result);
		else
			return evaluationContext->result;
	}
}

Term * Eval2(Term * term) {
	return Evaluate(AcquireTermEvaluationContext(0, AcquireContextBindings(), term));
}