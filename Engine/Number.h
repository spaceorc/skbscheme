#pragma once

#include "Term.h"

Term * OperatorPlus(List arguments);
Term * OperatorMinus(List arguments);
Term * ParseNumber(LimitedStr symbol);
int IsNumber(LimitedStr symbol);
