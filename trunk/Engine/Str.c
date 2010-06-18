#include <string.h>

#include "Str.h"

int StrCompare(ConstLimitedStr first, ConstLimitedStr second) {
	if (first.size > second.size)
		return strncmp(first.str, second.str, first.size);
	return strncmp(first.str, second.str, second.size);
}

ConstLimitedStr LimitConstStr(ConstStr str) {
	ConstLimitedStr result;
	result.str = str;
	result.size = strlen(str);
	return result;
}