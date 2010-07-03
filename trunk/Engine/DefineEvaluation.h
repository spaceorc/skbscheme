#pragma once

#include "Evaluation.h"

typedef struct structDefineEvaluationContext DefineEvaluationContext;
typedef struct structDefineLambdaEvaluationContext DefineLambdaEvaluationContext;

struct structDefineEvaluationContext {
	EvaluationContextBase base;
	List arguments;
	LimitedStr name;
};

struct structDefineLambdaEvaluationContext {
	EvaluationContextBase base;
	List arguments;
};

DefineEvaluationContext * AllocateDefineEvaluationContext();
DefineLambdaEvaluationContext * AllocateDefineLambdaEvaluationContext();
EvaluationContextBase * AcquireDefineEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments);
EvaluationContextBase * AcquireDefineLambdaEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments);
