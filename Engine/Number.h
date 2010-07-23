#pragma once

#include "Term.h"

Term * OperatorPlus(List arguments);
Term * OperatorMinus(List arguments);
Term * OperatorNumberEq(List arguments);
Term * ParseNumber(LimitedStr variable);
int IsNumber(LimitedStr variable);
Term * FunctionIsNumber(List arguments);