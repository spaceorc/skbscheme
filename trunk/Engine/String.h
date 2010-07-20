#pragma once

#include "Term.h"
#include "Str.h"

Term * FunctionIsString(List arguments);
Term * FunctionMakeString(List arguments);
Term * FunctionStringLength(List arguments);
Term * FunctionStringRef(List arguments);
Term * FunctionStringSet(List arguments);
Term * FunctionSubstring(List arguments);