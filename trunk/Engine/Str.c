#include "Str.h"
#include "MemoryManager.h"

int CompareConstantLimitedStr(ConstantLimitedStr first, ConstantLimitedStr second) {
	Chr a, b;
	while (a = IterateChrConstantLimitedStr(&first)) {
		if (0 == (b = IterateChrConstantLimitedStr(&second)))
			return a;
		if (a != b)
			return a - b;
	}
	return -IterateChrConstantLimitedStr(&second);
}

int Compare(LimitedStr first, LimitedStr second) {
	return CompareConstantLimitedStr(ConstLimitedStr(first), ConstLimitedStr(second));
}

LimitedStr Concatenate(LimitedStr first, LimitedStr second) {
	return ConcatenateConstantLimitedStr(ConstLimitedStr(first), ConstLimitedStr(second));
}

LimitedStr ConcatenateConstantLimitedStr(ConstantLimitedStr first, ConstantLimitedStr second) {
	LimitedStr result = AllocateLimitedStr(first.size + second.size - 1);
	Chr * to = result.str, from;
	while(from = IterateChrConstantLimitedStr(&first))
		*(to++) = from;
	while(from = IterateChrConstantLimitedStr(&second))
		*(to++) = from;
	*to = 0;
	return result;
}

ConstantLimitedStr LimitConstantStr(ConstantStr str) {
	ConstantLimitedStr result;
	result.str = str;
	for(result.size = 0;0 != *str; str++, result.size++);
	return result;
}

ConstantLimitedStr ConstLimitedStr(LimitedStr str) {
	ConstantLimitedStr result;
	result.str = str.str;
	result.size = str.size;
	return result;
}

Chr IterateChrConstantStr(ConstantStr * str, unsigned int * size) {
	Chr result = 0;
	if (((*size) > 0) && (0 != **str)){
		result = **str;
		(*str)++;
		(*size)--;
	}
	return result;
}

Chr IterateChrConstantLimitedStr(ConstantLimitedStr * str) {
	return IterateChrConstantStr(&(str->str), &(str->size));
}

Chr CurrentChr(LimitedStr * str) {
	if (str->size < 1)
		return 0;
	return *(str->str);
}

Chr IterateChr(LimitedStr * str) {
	return IterateChrConstantStr(&(str->str), &(str->size));
}

LimitedStr AllocateLimitedStr(unsigned int size) {
	LimitedStr result;
	result.str = AllocateString(size);
	result.size = size;
	return result;
}

LimitedStr LimitedStrFromConstantStr(ConstantStr str) {
	return LimitedStrFromConstantLimitedStr(LimitConstantStr(str));
}

LimitedStr LimitedStrFromConstantLimitedStr(ConstantLimitedStr str) {
	LimitedStr result = AllocateLimitedStr(str.size + 1);
	char from, *to = result.str;
	while (from = IterateChrConstantLimitedStr(&str))
		*(to++) = from;
	*to = 0;
	return result;
}

LimitedStr DeepCopy(LimitedStr str) {
	return ConcatenateConstantLimitedStr(ConstLimitedStr(str), LimitConstantStr(""));
}

StringBuilder * AppendLimitedStr(StringBuilder * stringBuilder, LimitedStr str) {
	return AppendConstantLimitedStr(stringBuilder, ConstLimitedStr(str));
}

StringBuilder * AppendConstantStr(StringBuilder * stringBuilder, ConstantStr str) {
	return AppendConstantLimitedStr(stringBuilder, LimitConstantStr(str));
}

StringBuilder * AppendConstantLimitedStr(StringBuilder * stringBuilder, ConstantLimitedStr str) {
	StringBuilder * result = AllocateStringBuilder(stringBuilder, str.size);
	StringBuffer buffer = result->stringBuffer;
	Chr current;
	while (current = IterateChrConstantLimitedStr(&str))
		buffer.buffer[buffer.offset++] = current;
	return result;
}

StringBuilder * AppendChr(StringBuilder * stringBuilder, Chr chr) {
	StringBuilder * result = stringBuilder;
	StringBuffer * buffer = result ? &(result->stringBuffer) : 0;
	if (buffer && buffer->offset + 1 >= buffer->size) {
		result = AllocateStringBuilder(result, 100);
		buffer = &(result->stringBuffer);
	}
	buffer->buffer[buffer->offset++] = chr;
	return result;
}

LimitedStr BuildString(StringBuilder * stringBuilder) {
	unsigned int size = 0;
	StringBuilder * current = stringBuilder;
	LimitedStr result;
	while (current) {
		size += current->stringBuffer.offset;
		current = current->next;
	}
	result = AllocateLimitedStr(size);
	return result;
}