#pragma once

typedef const char * ConstStr;

struct structConstLimitedStr;
typedef struct structConstLimitedStr ConstLimitedStr;

struct structConstLimitedStr {
	ConstStr str;
	unsigned int size;
};

int StrCompare(ConstLimitedStr first, ConstLimitedStr second);
ConstLimitedStr LimitConstStr(ConstStr str);