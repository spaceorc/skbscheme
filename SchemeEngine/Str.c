#include <assert.h>

#include "Str.h"
#include "MemoryManager.h"

int CompareConstantLimitedStr(ConstantLimitedStr first, ConstantLimitedStr second) {
	Chr a, b;
	while (a = IterateChrConstantLimitedStr(&first)) {
		if ('\0' == (b = IterateChrConstantLimitedStr(&second)))
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
	LimitedStr result = CreateLimitedStr(first.size + second.size + 1);
	Chr * to = result.str, from;
	while(from = IterateChrConstantLimitedStr(&first))
		*(to++) = from;
	while(from = IterateChrConstantLimitedStr(&second))
		*(to++) = from;
	*to = '\0';
	return result;
}

ConstantLimitedStr LimitConstantStr(ConstantStr str) {
	ConstantLimitedStr result;
	result.str = str;
	for(result.size = 0;'\0' != *str; str++, result.size++);
	return result;
}

ConstantLimitedStr ConstLimitedStr(LimitedStr str) {
	ConstantLimitedStr result;
	result.str = str.str;
	result.size = str.size;
	return result;
}

Chr IterateChrConstantStr(ConstantStr * str, unsigned int * size) {
	Chr result = '\0';
	if (((*size) > 0) && ('\0' != **str)){
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

LimitedStr CreateLimitedStr(unsigned int size) {
	LimitedStr result;
	result.str = AllocateString(size);
	result.size = size;
	return result;
}

LimitedStr LimitedStrFromConstantStr(ConstantStr str) {
	return LimitedStrFromConstantLimitedStr(LimitConstantStr(str));
}

LimitedStr LimitedStrFromConstantLimitedStr(ConstantLimitedStr str) {
	LimitedStr result = CreateLimitedStr(str.size + 1);
	char from, *to = result.str;
	while (from = IterateChrConstantLimitedStr(&str))
		*(to++) = from;
	*to = '\0';
	return result;
}

LimitedStr ZeroStr(LimitedStr str) {
	if (str.str[str.size - 1])
		return ConcatenateConstantLimitedStr(ConstLimitedStr(str), LimitConstantStr(""));
	return str;
}

unsigned int Length(LimitedStr str) {
	Chr * current = str.str;
	unsigned int result = 0;
	while (str.size-- && *(str.str++))
		result++;
	return result;
}