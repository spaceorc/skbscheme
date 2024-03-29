#pragma once

#include "Term.h"
#include "MemoryManager.h"

Term * Error(LimitedStr str);
Term * InvalidVariable(LimitedStr variable);
Term * InvalidClosingBracket();
Term * InvalidArgumentCount();
Term * InvalidArgumentType();
Term * FunctionError(List arguments);
Term * BadSyntax();
Term * PosixError(int errorNumber);
Term * ContractError();