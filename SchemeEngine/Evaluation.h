#pragma once

#include "Term.h"

void FillEvaluationContextBase(EvaluationContextBase * base, EvaluationContextBase * parent, ContextBindings * contextBindings, ChildEvaluatedPtr childEvaluated, EvaluatePtr evaluate);
Term * Evaluate(EvaluationContextBase * evaluationContext);

Term * EvalIterative(Term * term, ContextBindings * contextBindings);

#define THIS_CONTEXT ((EvaluationContextBase *) evaluationContext)
