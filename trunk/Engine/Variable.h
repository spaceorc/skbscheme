#pragma once

#include "Term.h"

Term * Variable(LimitedStr str);
Term * VariableFromConstantStr(ConstantStr str);
Term * VariableFromConstantLimitedStr(ConstantLimitedStr str);