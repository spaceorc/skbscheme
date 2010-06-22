#pragma once

#include "Term.h"

Term * Eval(Term * term, ContextBindings * contextBindings);
Term * EvalList(List list, ContextBindings * contextBindings);
ContextBindings * AcquireContextBindings();
Term * InvalidSymbol();