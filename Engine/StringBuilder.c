#include <assert.h>

#include "StringBuilder.h"

StringBuilder AllocateStringBuilder() {
	return 0;
}

StringBuilder AppendChr(StringBuilder stringBuilder, Chr chr) {
	return PushList(stringBuilder, Character(chr));
}

unsigned int CalculateStringLength(StringBuilder stringBuilder) {
	unsigned int result = 0;
	while (IterateList(&stringBuilder))
		result++;
	return result;
}

LimitedStr BuildString(StringBuilder stringBuilder) {
	LimitedStr result = CreateLimitedStr(CalculateStringLength(stringBuilder) + 1);
	Chr * current = result.str + result.size;
	Term * character = 0;
	*(--current) = 0;
	while (character = IterateList(&stringBuilder)) {
		assert(terCharacter == character->tag);
		*(--current) = character->character;
	}
	return result;
}