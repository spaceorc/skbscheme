#pragma once

#include "Term.h"

Term * True();
Term * False();
int IsTrue(Term * term);
int IsFalse(Term * term);
Term * LazyFunctionAnd(List arguments, ContextBindings * contextBindings);
Term * LazyFunctionOr(List arguments, ContextBindings * contextBindings);
Term * LazyFunctionIf(List arguments, ContextBindings * contextBindings);