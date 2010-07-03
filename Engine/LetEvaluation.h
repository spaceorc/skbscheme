#pragma once

#include "Evaluation.h"

typedef struct structLetEvaluationContext LetEvaluationContext;

struct structLetEvaluationContext {
	EvaluationContextBase base;
	List arguments;
	List letsList;
	LimitedStr currentLetSymbol;
	ContextBindings * childContextBindings;
};

LetEvaluationContext * AllocateLetEvaluationContext();
EvaluationContextBase * AcquireLetEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments);
