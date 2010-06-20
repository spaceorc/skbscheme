#pragma once

#include "Term.h"

Term * FunctionCons(List arguments);
Term * FunctionCar(List arguments);
Term * FunctionCdr(List arguments);

List InternalAppend(List list, Term * term);
