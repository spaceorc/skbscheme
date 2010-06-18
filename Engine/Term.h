#pragma once

#include "Str.h"

#define tagNumber 0
#define tagFunction 1
#define tagPair 2
#define tagError 3
#define tagNil 4
#define tagRedex 5
#define tagConstantString 6

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
Term * InvalidSymbol();
Term * Nil();
Term * IterateList(List * iterator);
List AppendListElement(List list, Term * term);
int TakeArguments(List from, Term * to[], int atLeast, int atMost, Term ** error);
Term * Function(FunctionPtr function);
Term * ConstantString(ConstStr str);
Term * ConstantStringFromLimited(ConstLimitedStr str);

#define TakeSeveralArguments(from, to, error) TakeArguments(from, to, sizeof(to)/sizeof(to[0]), sizeof(to)/sizeof(to[0]), error)
#define TakeSingleArgument(from, to, error) TakeArguments(from, to, 1, 1, error)