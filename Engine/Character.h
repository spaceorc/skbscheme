#pragma once

#include "Term.h"

Term * ParseCharacter(LimitedStr symbol);
int IsCharacter(LimitedStr symbol);
Term * FunctionIsCharacter(List arguments);
Term * OperatorCharacterEq(List arguments);