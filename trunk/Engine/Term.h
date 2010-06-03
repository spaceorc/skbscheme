#pragma once

#define tagNumber 0
#define tagFunction 1
#define tagPair 2
#define tagError 3
#define tagNil 4

const char * DumpTag(int tag);

struct structPair;
struct structTerm;
typedef struct structTerm * (*FunctionPtr)(struct structPair * arguments);

typedef struct structTerm
{
	int tag;
	union {
		FunctionPtr function;
		int number;
		struct structPair * pair;
		const char * message;
	};
} Term;

typedef struct structPair
{
	Term * first;
	Term * second;
} Pair;

Term * AllocateTerm(int tag);
Pair * AllocatePair();
void ReleaseTerm(Term * term);
void ReleasePair(Pair * pair);
Term * InvalidArgumentCount();
Term * InvalidArgumentType();
Term * Nil();
Term * IterateList(Pair ** iterator);
int TakeArguments(Pair * from, Term * to[], int atLeast, int atMost, Term ** error);
Term * Function(FunctionPtr function);

#define TakeSeveralArguments(from, to, error) TakeArguments(from, to, sizeof(to)/sizeof(to[0]), sizeof(to)/sizeof(to[0]), error)
#define TakeSingleArgument(from, to, error) TakeArguments(from, to, 1, 1, error)