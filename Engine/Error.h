#pragma once

#include "Term.h"
#include "MemoryManager.h"

Term * InvalidSymbol(LimitedStr symbol);
Term * InvalidClosingBracket();
Term * InvalidArgumentCount();
Term * InvalidArgumentType();
Term * FunctionError(List arguments);
Term * BadSyntax();
Term * PosixError(int errorNumber);
Term * ContractError();