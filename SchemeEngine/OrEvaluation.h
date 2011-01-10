#pragma once

#include "Evaluation.h"

typedef struct structOrEvaluationContext OrEvaluationContext;

struct structOrEvaluationContext {
	EvaluationContextBase base;	
	List arguments;
};

OrEvaluationContext * AllocateOrEvaluationContext();
EvaluationContextBase * AcquireOrEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments);
