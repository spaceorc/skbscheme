#pragma once

#include "Term.h"

Term * OperatorPlus(List arguments);
Term * OperatorMinus(List arguments);
Term * parseNumber(LimitedStr symbol);
int isNumber(LimitedStr symbol);
