#pragma once

typedef unsigned char Chr;
typedef const Chr * ConstantStr;

struct structConstantLimitedStr;
typedef struct structConstantLimitedStr ConstantLimitedStr;

struct structConstantLimitedStr {
	ConstantStr str;
	unsigned int size;
};

struct structLimitedStr;
typedef struct structLimitedStr LimitedStr;

struct structLimitedStr {
	Chr * str;
	unsigned int size;
};

int CompareConstantLimitedStr(ConstantLimitedStr first, ConstantLimitedStr second);
int Compare(LimitedStr first, LimitedStr second);
LimitedStr Concatenate(LimitedStr first, LimitedStr second);
LimitedStr ConcatenateConstantLimitedStr(ConstantLimitedStr first, ConstantLimitedStr second);
ConstantLimitedStr LimitConstantStr(ConstantStr str);
ConstantLimitedStr ConstLimitedStr(LimitedStr str);
Chr IterateChrConstantLimitedStr(ConstantLimitedStr * str);
Chr IterateChr(LimitedStr * str);
Chr CurrentChr(LimitedStr * str);
LimitedStr AllocateLimitedStr(unsigned int size);
LimitedStr LimitedStrFromConstantStr(ConstantStr str);
LimitedStr LimitedStrFromConstantLimitedStr(ConstantLimitedStr str);
LimitedStr DeepCopy(LimitedStr str);