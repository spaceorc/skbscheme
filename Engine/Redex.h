#pragma once

#include "Term.h"

Term * Eval(Term * term, ContextBindings * contextBindings);
ContextBindings * AcquireContextBindings();
ContextBindings * AllocateContextBindings(ContextBindings * previous);