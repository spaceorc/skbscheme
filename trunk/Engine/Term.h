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

const char * DumpTag(int tag);

struct structTerm;
struct structPair;
struct structContextBindings;
typedef struct structTerm Term;
typedef struct structPair Pair, *List;
typedef struct structContextBindings ContextBindings;
typedef struct structDefinedFunction DefinedFunction;
typedef Term * (*FunctionPtr)(List arguments);
typedef Term * (*LazyFunctionPtr)(List arguments, ContextBindings * contextBindings);

struct structDefinedFunction {
	List formalArguments;
	Term * function;
};

struct structTerm {
	int tag;
	union {
		FunctionPtr function;
		int number;
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

Term * AllocateTerm(int tag);
Pair * AllocatePair();
Term * InvalidArgumentCount();
Term * InvalidArgumentType();
Term * Nil();
Term * IterateList(List * iterator);
int TakeArguments(List from, Term * to[], int atLeast, int atMost, Term ** error);
Term * Function(FunctionPtr function);
Term * LazyFunction(LazyFunctionPtr lazyFunction);
Term * DefineFunction(List formalArguments, Term * function);
Term * ConstantStringFromConstantStr(ConstantStr str);
Term * ConstantStringFromConstantLimitedStr(ConstantLimitedStr str);
Term * ConstantString(LimitedStr str);
Term * Symbol(LimitedStr str);
Term * SymbolFromConstantStr(ConstantStr str);
Term * SymbolFromConstantLimitedStr(ConstantLimitedStr str);

#define TakeSeveralArguments(from, to, error) TakeArguments(from, to, sizeof(to)/sizeof(to[0]), sizeof(to)/sizeof(to[0]), error)
#define TakeSingleArgument(from, to, error) TakeArguments(from, to, 1, 1, error)

ContextBindings * AllocateContextBindings(ContextBindings * previous);