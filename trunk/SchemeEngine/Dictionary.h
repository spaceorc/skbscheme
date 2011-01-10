#pragma once

#include "Pair.h"

typedef List Dictionary;

Dictionary CreateDictionary();
Term * Find(Dictionary dictionary, LimitedStr key);
Term * FindConstantStr(Dictionary dictionary, ConstantStr key);
Term * FindConstantLimitedStr(Dictionary dictionary, ConstantLimitedStr key);
Dictionary Set(Dictionary dictionary, LimitedStr key, Term * value);
Dictionary SetConstantStr(Dictionary dictionary, ConstantStr key, Term * value);
Dictionary SetConstantLimitedStr(Dictionary dictionary, ConstantLimitedStr key, Term * value);