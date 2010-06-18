#include "Dictionary.h"

Term * InternalFind(List dictionary, ConstLimitedStr key) {
	Term * keyValuePair = 0;
	while(keyValuePair = IterateList(&dictionary)) {
		if (terPair != keyValuePair->tag)
			return InvalidArgumentType();
		if (terConstantString != keyValuePair->pair->first->tag)
			return InvalidArgumentType();
			if (0 == StrCompare(keyValuePair->pair->first->constStr, key))
				return keyValuePair->pair->second;
	}
	return 0;
}

List InternalSetFromLimited(List dictionary, ConstLimitedStr key, Term * value) {
	return InternalAppend(dictionary, InternalCons(ConstantStringFromLimited(key), value));
}

List InternalSet(List dictionary, ConstStr key, Term * value) {
	return InternalAppend(dictionary, InternalCons(ConstantString(key), value));
}