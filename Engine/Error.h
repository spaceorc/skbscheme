#pragma once

#include "Term.h"
#include "Memory.h"

Term * InvalidSymbol(LimitedStr symbol);
Term * InvalidClosingBracket();
Term * InvalidArgumentCount();
Term * InvalidArgumentType();
Term * FunctionError(List arguments);
Term * BadSyntax();
Term * RaisePosixError(int errorNumber);