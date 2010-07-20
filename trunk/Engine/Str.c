#include <assert.h>

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
	LimitedStr result = AllocateLimitedStr(first.size + second.size + 1);
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

Chr IterateChr(LimitedStr * str) {
	return IterateChrConstantStr(&(str->str), &(str->size));
}

void UnwindChr(LimitedStr * str) {
	str->size++;
	str->str--;
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

LimitedStr ZeroStr(LimitedStr str) {
	return ConcatenateConstantLimitedStr(ConstLimitedStr(str), LimitConstantStr(""));
}

unsigned int Length(LimitedStr str) {
	Chr * current = str.str;
	unsigned int result = 0;
	while (str.size-- && *(str.str++))
		result++;
	return result;
}

StringBuilder * AppendLimitedStr(StringBuilder * stringBuilder, LimitedStr str) {
	return AppendConstantLimitedStr(stringBuilder, ConstLimitedStr(str));
}

StringBuilder * AppendConstantStr(StringBuilder * stringBuilder, ConstantStr str) {
	return AppendConstantLimitedStr(stringBuilder, LimitConstantStr(str));
}

StringBuilder * AppendConstantLimitedStr(StringBuilder * stringBuilder, ConstantLimitedStr str) {
	StringBuilder * result = AllocateStringBuilder(stringBuilder, str.size);
	StringBuffer * buffer = &(result->stringBuffer);
	Chr current;
	while (current = IterateChrConstantLimitedStr(&str))
		buffer->buffer[buffer->offset++] = current;
	buffer->size = buffer->offset;
	return result;
}

StringBuilder * AppendChr(StringBuilder * stringBuilder, Chr chr) {
	StringBuilder * result = stringBuilder;
	StringBuffer * buffer = result ? &(result->stringBuffer) : 0;
	if (!buffer || buffer->offset >= buffer->size) {
		result = AllocateStringBuilder(result, 100);
		buffer = &(result->stringBuffer);
	}
	buffer->buffer[buffer->offset++] = chr;
	return result;
}

unsigned int CalculateStringLength(StringBuilder * stringBuilder) {
	unsigned int size = 0;
	while (stringBuilder) {
		size += stringBuilder->stringBuffer.offset;
		stringBuilder = stringBuilder->next;
	}
	return size;
}

LimitedStr BuildString(StringBuilder * stringBuilder) {
	unsigned int size = CalculateStringLength(stringBuilder);
	LimitedStr result = AllocateLimitedStr(size + 1);
	Chr * current = result.str + size;
	*current = 0;
	while (stringBuilder) {
		StringBuffer buffer = stringBuilder->stringBuffer;
		Chr * from = buffer.buffer;
		Chr * to = current - buffer.offset;
		current = to;
		while (buffer.offset--)
			*(to++) = *(from++);
		stringBuilder = stringBuilder->next;
	}
	assert(current == result.str);
	return result;
}