#pragma once

#include "Term.h"
#include "Redex.h"

Term * LazyFunctionLet(List arguments, ContextBindings * contextBindings);
Term * LazyFunctionDefine(List arguments, ContextBindings * contextBindings);