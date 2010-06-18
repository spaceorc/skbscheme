#pragma once

#include "Pair.h"

Term * InternalFind(List dictionary, ConstLimitedStr key);
List InternalSetFromLimited(List dictionary, ConstLimitedStr key, Term * value);
List InternalSet(List dictionary, ConstStr key, Term * value);