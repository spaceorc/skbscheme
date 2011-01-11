#include "Dictionary.h"
#include "List.h"
#include "Pair.h"
#include "String.h"

Dictionary CreateDictionary() {
	return NULL;
}

Term * FindConstantStr(Dictionary dictionary, ConstantStr key) {
	return FindConstantLimitedStr(dictionary, LimitConstantStr(key));
}

Term * FindConstantLimitedStr(Dictionary dictionary, ConstantLimitedStr key) {
	Term * keyValuePair = NULL;
	while(keyValuePair = Iterate(&dictionary)) {
		if (terPair != keyValuePair->tag)
			return InvalidArgumentType();
		if (terString != keyValuePair->pair->first->tag)
			return InvalidArgumentType();
		if (0 == CompareConstantLimitedStr(ConstLimitedStr(keyValuePair->pair->first->string), key))
			return keyValuePair->pair->second;
	}
	return NULL;
}

Term * Find(Dictionary dictionary, LimitedStr key) {
	return FindConstantLimitedStr(dictionary, ConstLimitedStr(key));
}

List Set(Dictionary dictionary, LimitedStr key, Term * value) {
	return Push(dictionary, Cons(String(key), value));
}

Dictionary SetConstantStr(Dictionary dictionary, ConstantStr key, Term * value) {
	return Push(dictionary, Cons(StringFromConstantStr(key), value));
}

Dictionary SetConstantLimitedStr(Dictionary dictionary, ConstantLimitedStr key, Term * value) {
	return Push(dictionary, Cons(StringFromConstantLimitedStr(key), value));
}