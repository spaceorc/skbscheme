#pragma once

#include "Term.h"

Term * FunctionCons(Pair * arguments);
Term * FunctionCar(Pair * arguments);
Term * FunctionCdr(Pair * arguments);
Term * FunctionApply(Pair * arguments);