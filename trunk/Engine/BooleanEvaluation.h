#pragma once

#include "Evaluation.h"

EvaluationContextBase * AllocateEmptyLazyEvaluationContext();
EvaluationContextBase * AcquireEmptyLazyEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments);
