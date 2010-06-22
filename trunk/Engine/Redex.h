#pragma once

#include "Term.h"

Term * Eval(Term * term, ContextBindings * contextBindings);
Term * EvalInDefineMode(Term * term, ContextBindings * contextBindings);
ContextBindings * AcquireContextBindings();
Term * InvalidSymbol();