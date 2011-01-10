#pragma once

#include "Evaluation.h"

typedef struct structCondEvaluationContext CondEvaluationContext;

struct structCondEvaluationContext {
	EvaluationContextBase base;	
	List arguments;
	List condItemArguments;
};

CondEvaluationContext * AllocateCondEvaluationContext();
EvaluationContextBase * AcquireCondEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments);
