#pragma once

#include "Str.h"

#define terNumber 0
#define terFunction 1
#define terPair 2
#define terError 3
#define terNil 4
#define terRedex 5
#define terConstantString 6

const char * DumpTag(int tag);

struct structPair;
struct structTerm;
typedef struct structPair Pair, *List;
typedef struct structTerm Term;
typedef Term * (*FunctionPtr)(List arguments);

struct structTerm
{
	int tag;
	union {
		FunctionPtr function;
		int number;
		Pair * pair;
		List redex;
		const char * message;
		ConstLimitedStr constStr;
	};
};

struct structPair
{
	Term * first;
	Term * second;
};

Term * AllocateTerm(int tag);
Pair * AllocatePair();
Term * InvalidArgumentCount();
Term * InvalidArgumentType();
Term * Nil();
Term * IterateList(List * iterator);
int TakeArguments(List from, Term * to[], int atLeast, int atMost, Term ** error);
Term * Function(FunctionPtr function);
Term * ConstantString(ConstStr str);
Term * ConstantStringFromLimited(ConstLimitedStr str);

#define TakeSeveralArguments(from, to, error) TakeArguments(from, to, sizeof(to)/sizeof(to[0]), sizeof(to)/sizeof(to[0]), error)
#define TakeSingleArgument(from, to, error) TakeArguments(from, to, 1, 1, error)