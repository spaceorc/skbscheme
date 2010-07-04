#pragma once

#include "Evaluation.h"

typedef struct structIfEvaluationContext IfEvaluationContext;

struct structIfEvaluationContext {
	EvaluationContextBase base;	
	List arguments;
};

IfEvaluationContext * AllocateIfEvaluationContext();
EvaluationContextBase * AcquireIfEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments);
