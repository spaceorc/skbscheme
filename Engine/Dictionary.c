#include "Dictionary.h"

Dictionary CreateDictionary() {
	return 0;
}

Term * FindConstantStr(Dictionary dictionary, ConstantStr key) {
	return FindConstantLimitedStr(dictionary, LimitConstantStr(key));
}

Term * FindConstantLimitedStr(Dictionary dictionary, ConstantLimitedStr key) {
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

Term * Find(Dictionary dictionary, LimitedStr key) {
	return FindConstantLimitedStr(dictionary, ConstLimitedStr(key));
}

List Set(Dictionary dictionary, LimitedStr key, Term * value) {
	return PushList(dictionary, Cons(String(key), value));
}

Dictionary SetConstantStr(Dictionary dictionary, ConstantStr key, Term * value) {
	return PushList(dictionary, Cons(StringFromConstantStr(key), value));
}

Dictionary SetConstantLimitedStr(Dictionary dictionary, ConstantLimitedStr key, Term * value) {
	return PushList(dictionary, Cons(StringFromConstantLimitedStr(key), value));
}