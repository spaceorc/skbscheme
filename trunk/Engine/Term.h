#pragma once

extern const int tagNumber;
extern const int tagFunction;
extern const int tagPair;
extern const int tagError;
extern const int tagNil;

struct tagPair;
struct tagTerm;
typedef struct tagTerm (*Function)(struct tagPair * arguments);

typedef struct tagTerm
{
	int tag;
	union {
		Function * function;
		int number;
		struct tagPair * pair;
		const char * message;
	};
} Term;

typedef struct tagPair
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
Term * Cons(Term * first, Term * second);
Term * CarPair(Pair * pair);
Term * CdrPair(Pair * pair);
Term * Nil();