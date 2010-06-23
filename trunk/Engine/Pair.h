#pragma once

#include "Term.h"

Term * FunctionCons(List arguments);
Term * FunctionCar(List arguments);
Term * FunctionCdr(List arguments);

// todo strange "Internal..." function in header...
List InternalAppend(List list, Term * term);
