#pragma once

#include "Term.h"
#include "Redex.h"

Term * LazyFunctionLet(List arguments, ContextBindings * contextBindings, int inExpressionMode);
Term * LazyFunctionDefine(List arguments, ContextBindings * contextBindings, int inExpressionMode);
Term * LazyFunctionLambda(List arguments, ContextBindings * contextBindings, int inExpressionMode);