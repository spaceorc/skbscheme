#pragma once

#include "Term.h"

Term * Character(Chr character);
Term * ParseCharacter(LimitedStr variable);
int IsCharacter(LimitedStr variable);
Term * FunctionIsCharacter(List arguments);
Term * FunctionCharacterEq(List arguments);