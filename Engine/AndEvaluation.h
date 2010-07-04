#pragma once

#include "Evaluation.h"

typedef struct structAndEvaluationContext AndEvaluationContext;

struct structAndEvaluationContext {
	EvaluationContextBase base;	
	List arguments;
	Term * currentResult;
};

AndEvaluationContext * AllocateAndEvaluationContext();
EvaluationContextBase * AcquireAndEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments);
