#include <assert.h>

#include "StringBuilder.h"

StringBuilder AllocateStringBuilder() {
	return 0;
}

StringBuilder InternalAppendChr(StringBuilder stringBuilder, Chr chr) {
	return PushList(stringBuilder, Character(chr));
}

unsigned int CalculateStringLength(StringBuilder stringBuilder) {
	unsigned int result = 0;
	while (IterateList(&stringBuilder))
		result++;
	return result;
}

LimitedStr InternalBuildString(StringBuilder stringBuilder) {
	LimitedStr result = AllocateLimitedStr(CalculateStringLength(stringBuilder) + 1);
	Chr * current = result.str + result.size;
	Term * character = 0;
	*(--current) = 0;
	while (character = IterateList(&stringBuilder)) {
		assert(terCharacter == character->tag);
		*(--current) = character->character;
	}
	return result;
}