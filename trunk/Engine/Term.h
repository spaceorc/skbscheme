#pragma once

#include "Str.h"

#define terNumber 0
#define terFunction 1
#define terPair 2
#define terError 3
#define terNil 4
#define terRedex 5
#define terConstantString 6
#define terSymbol 7
#define terLazyFunction 8
#define terDefinedFunction 9

typedef struct structTerm Term;
typedef struct structPair Pair, *List;
typedef struct structContextBindings ContextBindings;
typedef struct structDefinedFunction DefinedFunction;
typedef struct structParserContext ParserContext;
typedef Term * (*FunctionPtr)(List arguments);
typedef Term * (*LazyFunctionPtr)(List arguments, ContextBindings * contextBindings);

struct structDefinedFunction {
	List formalArguments;
	Term * function;
};

struct structParserContext {
	List redex;
	ParserContext * previous;
};

struct structTerm {
	int tag;
	union {
		int number;
		FunctionPtr function;
		Pair * pair;
		LimitedStr message;
		List redex;
		LimitedStr constantString;
		LimitedStr symbol;
		LazyFunctionPtr lazyFunction;
		DefinedFunction definedFunction;
	};
};

struct structContextBindings {
	List dictionary;
	ContextBindings * previous;
};

struct structPair {
	Term * first;
	Term * second;
};

#include "Constructors.h"

Term * InvalidArgumentCount();
Term * InvalidArgumentType();

Term * IterateList(List * iterator);
int TakeArguments(List from, Term * to[], int atLeast, int atMost, Term ** error);

#define TakeSeveralArguments(from, to, error) TakeArguments(from, to, sizeof(to)/sizeof(to[0]), sizeof(to)/sizeof(to[0]), error)
#define TakeSingleArgument(from, to, error) TakeArguments(from, to, 1, 1, error)