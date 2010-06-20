#pragma once

#include "Term.h"

Term * Number(int n);
Term * OperatorPlus(List arguments);
Term * OperatorMinus(List arguments);
Term * parseNumber(ConstLimitedStr symbol);
int isNumber(ConstLimitedStr symbol);