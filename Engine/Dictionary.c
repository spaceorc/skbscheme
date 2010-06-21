#include "Dictionary.h"

Term * InternalFindConstantStr(List dictionary, ConstantStr key) {
	return InternalFindConstantLimitedStr(dictionary, LimitConstantStr(key));
}

Term * InternalFindConstantLimitedStr(List dictionary, ConstantLimitedStr key) {
	Term * keyValuePair = 0;
	while(keyValuePair = IterateList(&dictionary)) {
		if (terPair != keyValuePair->tag)
			return InvalidArgumentType();
		if (terConstantString != keyValuePair->pair->first->tag)
			return InvalidArgumentType();
		if (0 == CompareConstantLimitedStr(ConstLimitedStr(keyValuePair->pair->first->constantString), key))
			return keyValuePair->pair->second;
	}
	return 0;
}

Term * InternalFind(List dictionary, LimitedStr key) {
	return InternalFindConstantLimitedStr(dictionary, ConstLimitedStr(key));
}

List InternalSet(List dictionary, LimitedStr key, Term * value) {
	return InternalAppend(dictionary, InternalCons(ConstantString(key), value));
}

List InternalSetConstantStr(List dictionary, ConstantStr key, Term * value) {
	return InternalAppend(dictionary, InternalCons(ConstantStringFromConstantStr(key), value));
}

List InternalSetConstantLimitedStr(List dictionary, ConstantLimitedStr key, Term * value) {
	return InternalAppend(dictionary, InternalCons(ConstantStringFromConstantLimitedStr(key), value));
}