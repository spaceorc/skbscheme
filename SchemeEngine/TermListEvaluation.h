#pragma once

#include "Evaluation.h"

typedef struct structTermListEvaluationContext TermListEvaluationContext;

struct structTermListEvaluationContext {
	EvaluationContextBase base;
	List input;
	Term * currentResult;
};

TermListEvaluationContext * AllocateTermListEvaluationContext();
EvaluationContextBase * AcquireTermListEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List input);
