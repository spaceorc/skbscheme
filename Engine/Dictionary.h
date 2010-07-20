#pragma once

#include "Pair.h"

typedef List Dictionary;

Dictionary AllocateDictionary();
Term * InternalFind(Dictionary dictionary, LimitedStr key);
Term * InternalFindConstantStr(Dictionary dictionary, ConstantStr key);
Term * InternalFindConstantLimitedStr(Dictionary dictionary, ConstantLimitedStr key);
Dictionary InternalSet(Dictionary dictionary, LimitedStr key, Term * value);
Dictionary InternalSetConstantStr(Dictionary dictionary, ConstantStr key, Term * value);
Dictionary InternalSetConstantLimitedStr(Dictionary dictionary, ConstantLimitedStr key, Term * value);