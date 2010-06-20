#include <malloc.h>

#include "Str.h"

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
	result.str = malloc(size);
	result.str[0] = 0;
	result.size = size;
	return result;
}

LimitedStr LimitedStrFromConstantStr(ConstantStr str) {
	return LimitedStrFromConstantLimitedStr(LimitConstantStr(str));
}

LimitedStr LimitedStrFromConstantLimitedStr(ConstantLimitedStr str) {
	LimitedStr result = AllocateLimitedStr(str.size + 1);
	char from, *to = result.str;
	while (from = IterateChrConstantLimitedStr(&str)) {
		*to = from;
		to++;
	}
	*to = 0;
	return result;
}