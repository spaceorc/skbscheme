#pragma once

#include "Pair.h"

Term * InternalFind(List dictionary, LimitedStr key);
Term * InternalFindConstantStr(List dictionary, ConstantStr key);
Term * InternalFindConstantLimitedStr(List dictionary, ConstantLimitedStr key);
List InternalSet(List dictionary, LimitedStr key, Term * value);
List InternalSetConstantStr(List dictionary, ConstantStr key, Term * value);
List InternalSetConstantLimitedStr(List dictionary, ConstantLimitedStr key, Term * value);