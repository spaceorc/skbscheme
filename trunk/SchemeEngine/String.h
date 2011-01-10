#pragma once

#include "Term.h"
#include "Str.h"

Term * StringFromConstantStr(ConstantStr str);
Term * StringFromConstantLimitedStr(ConstantLimitedStr str);
Term * String(LimitedStr str);
Term * FunctionIsString(List arguments);
Term * FunctionMakeString(List arguments);
Term * FunctionStringLength(List arguments);
Term * FunctionStringRef(List arguments);
Term * FunctionStringSet(List arguments);
Term * FunctionSubstring(List arguments);
Term * FunctionStringEq(List arguments);