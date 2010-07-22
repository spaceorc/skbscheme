#include "Dictionary.h"

Dictionary AllocateDictionary() {
	return 0;
}

Term * InternalFindConstantStr(Dictionary dictionary, ConstantStr key) {
	return InternalFindConstantLimitedStr(dictionary, LimitConstantStr(key));
}

Term * InternalFindConstantLimitedStr(Dictionary dictionary, ConstantLimitedStr key) {
	Term * keyValuePair = 0;
	while(keyValuePair = IterateList(&dictionary)) {
		if (terPair != keyValuePair->tag)
			return InvalidArgumentType();
		if (terString != keyValuePair->pair->first->tag)
			return InvalidArgumentType();
		if (0 == CompareConstantLimitedStr(ConstLimitedStr(keyValuePair->pair->first->string), key))
			return keyValuePair->pair->second;
	}
	return 0;
}

Term * InternalFind(Dictionary dictionary, LimitedStr key) {
	return InternalFindConstantLimitedStr(dictionary, ConstLimitedStr(key));
}

List InternalSet(Dictionary dictionary, LimitedStr key, Term * value) {
	return PushList(dictionary, InternalCons(ConstantString(key), value));
}

Dictionary InternalSetConstantStr(Dictionary dictionary, ConstantStr key, Term * value) {
	return PushList(dictionary, InternalCons(ConstantStringFromConstantStr(key), value));
}

Dictionary InternalSetConstantLimitedStr(Dictionary dictionary, ConstantLimitedStr key, Term * value) {
	return PushList(dictionary, InternalCons(ConstantStringFromConstantLimitedStr(key), value));
}