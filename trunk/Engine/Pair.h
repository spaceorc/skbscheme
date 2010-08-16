#pragma once

#include "Term.h"

Term * Cons(Term * first, Term * second);
Term * FunctionCons(List arguments);
Term * FunctionCar(List arguments);
Term * FunctionCdr(List arguments);
Term * FunctionIsNull(List arguments);
Term * FunctionIsPair(List arguments);
