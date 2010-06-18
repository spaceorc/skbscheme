#pragma once

#include "Term.h"

struct structContextBindings;
typedef struct structContextBindings ContextBindings;

struct structContextBindings {
	List dictionary;
	ContextBindings * previous;
};

Term * Eval(Term * term, ContextBindings * contextBindings);
ContextBindings * AcquireContextBindings();