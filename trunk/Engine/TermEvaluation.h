#pragma once

#include "Evaluation.h"

typedef struct structTermEvaluationContext TermEvaluationContext;

struct structTermEvaluationContext {
	EvaluationContextBase base;
	Term * input;
};

TermEvaluationContext * AllocateTermEvaluationContext();
EvaluationContextBase * AcquireTermEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, Term * input);
