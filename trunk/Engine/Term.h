#pragma once

#define tagNumber 0
#define tagFunction 1
#define tagPair 2
#define tagError 3
#define tagNil 4

const char * DumpTag(int tag);

struct structPair;
struct structTerm;
typedef struct structTerm (*Function)(struct structPair * arguments);

typedef struct structTerm
{
	int tag;
	union {
		Function * function;
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