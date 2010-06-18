#pragma once

#include "Term.h"

Term * InternalCons(Term * first, Term * second);

Term * FunctionCons(List arguments);
Term * FunctionCar(List arguments);
Term * FunctionCdr(List arguments);

List InternalAppend(List list, Term * term);
