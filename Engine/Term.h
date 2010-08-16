#pragma once

#include "Str.h"

typedef struct structTerm Term;
typedef struct structPair Pair, * List;
typedef struct structContextBindings ContextBindings;
typedef struct structLambda Lambda;
typedef struct structLazy Lazy;
typedef struct structEvaluationContextBase EvaluationContextBase;
typedef Term * (* FunctionPtr)(List arguments);
typedef Term * (* LazyFunctionPtr)(List arguments, ContextBindings * contextBindings);
typedef Term * (* CreateConstantPtr)();

typedef EvaluationContextBase * (* AcquireLazyEvaluationContextPtr)(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments);
typedef EvaluationContextBase * (* ChildEvaluatedPtr)(EvaluationContextBase * evaluationContext, Term * childResult);
typedef EvaluationContextBase * (* EvaluatePtr)(EvaluationContextBase * evaluationContext);

struct structEvaluationContextBase {
	EvaluationContextBase * parent;
	Term * result;
	ChildEvaluatedPtr childEvaluated;
	EvaluatePtr evaluate;
	ContextBindings * contextBindings;
};

struct structContextBindings {
	List dictionary;
	ContextBindings * previous;
};

struct structLambda {
	List formalArguments;
	List body;
	ContextBindings * context;
};

struct structLazy {
	LazyFunctionPtr function;
	AcquireLazyEvaluationContextPtr acquireEvaluationContext;
};

struct structTerm {
	int tag;
	union {
		int number;
		FunctionPtr function;
		Pair * pair;
		LimitedStr message;
		List redex;
		LimitedStr string;
		LimitedStr variable;
		Lazy lazy;
		Lambda lambda;
		int boolean;
		Chr character;
		int fildes;
	};
};

/*! \enum Tag of term
 * Each term has a tag specifying the way the content of term is to be treated.
 */
#define terNumber 0
#define terFunction 1
#define terPair 2
#define terError 3
#define terNil 4
#define terRedex 5
#define terString 6
#define terVariable 7
#define terLazyFunction 8
#define terLambda 9
#define terVoid 10
#define terBoolean 11
#define terCharacter 12
#define terFileDescriptor 13

/*! \struct Pair
 * Pair contains of pointers to first and second Term. List is a pointer to pair.
 */
struct structPair {
	Term * first;
	Term * second;
};