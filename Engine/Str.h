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

int Compare(LimitedStr first, LimitedStr second);
int CompareConstantLimitedStr(ConstantLimitedStr first, ConstantLimitedStr second);

LimitedStr Concatenate(LimitedStr first, LimitedStr second);
LimitedStr ConcatenateConstantLimitedStr(ConstantLimitedStr first, ConstantLimitedStr second);

ConstantLimitedStr LimitConstantStr(ConstantStr str);
ConstantLimitedStr ConstLimitedStr(LimitedStr str);
LimitedStr LimitedStrFromConstantStr(ConstantStr str);
LimitedStr LimitedStrFromConstantLimitedStr(ConstantLimitedStr str);

Chr IterateChr(LimitedStr * str);
Chr IterateChrConstantLimitedStr(ConstantLimitedStr * str);

void UnwindChr(LimitedStr * str);

LimitedStr AllocateLimitedStr(unsigned int size);

LimitedStr ZeroStr(LimitedStr str);

unsigned int Length(LimitedStr str);