#pragma once

#include "Term.h"

typedef struct structEvaluationContextBase EvaluationContextBase;
typedef EvaluationContextBase * (*ChildEvaluatedPtr)(EvaluationContextBase * evaluationContext, Term * childResult);
typedef EvaluationContextBase * (*EvaluatePtr)(EvaluationContextBase * evaluationContext);

struct structEvaluationContextBase {
	EvaluationContextBase * parent;
	Term * result;
	ChildEvaluatedPtr childEvaluated;
	EvaluatePtr evaluate;
	ContextBindings * contextBindings;
};

void FillEvaluationContextBase(EvaluationContextBase * base, EvaluationContextBase * parent, ContextBindings * contextBindings, ChildEvaluatedPtr childEvaluated, EvaluatePtr evaluate);
Term * Evaluate(EvaluationContextBase * evaluationContext);

Term * Eval2(Term * term);
