#pragma once

#include "Evaluation.h"

typedef struct structReductionContext ReductionContext;

struct structReductionContext {
	EvaluationContextBase base;
	List redex;
	List arguments;
	Term * function;
};

ReductionContext * AllocateReductionContext();
EvaluationContextBase * AcquireReductionContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List redex);
