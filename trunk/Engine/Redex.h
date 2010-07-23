#pragma once

#include "Term.h"

Term * EvalRecursive(Term * term, ContextBindings * contextBindings);
Term * EvalList(List list, ContextBindings * contextBindings);
Term * Resolve(ContextBindings * contextBindings, LimitedStr variable);