#pragma once

#include "Str.h"

#define terNumber 0
#define terFunction 1
#define terPair 2
#define terError 3
#define terNil 4
#define terRedex 5
#define terString 6
#define terSymbol 7
#define terLazyFunction 8
#define terLambda 9
#define terEmpty 10
#define terBoolean 11
#define terCharacter 12
#define terFileDescriptor 13

typedef struct structTerm Term;
typedef struct structPair Pair, *List;
typedef struct structContextBindings ContextBindings;
typedef struct structLambda Lambda;
typedef struct structLazy Lazy;
typedef struct structParserContext ParserContext;
typedef struct structEvaluationContextBase EvaluationContextBase;
typedef Term * (*FunctionPtr)(List arguments);
typedef Term * (*LazyFunctionPtr)(List arguments, ContextBindings * contextBindings);
typedef Term * (*CreateConstantPtr)();

typedef EvaluationContextBase * (*AcquireLazyEvaluationContextPtr)(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments);
typedef EvaluationContextBase * (*ChildEvaluatedPtr)(EvaluationContextBase * evaluationContext, Term * childResult);
typedef EvaluationContextBase * (*EvaluatePtr)(EvaluationContextBase * evaluationContext);

struct structEvaluationContextBase {
	EvaluationContextBase * parent;
	Term * result;
	ChildEvaluatedPtr childEvaluated;
	EvaluatePtr evaluate;
	ContextBindings * contextBindings;
};

struct structParserContext {
	List redex;
	ParserContext * previous;
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
		LimitedStr symbol;
		Lazy lazy;
		Lambda lambda;
		int boolean;
		Chr character;
		int fildes;
	};
};

struct structPair {
	Term * first;
	Term * second;
};

#include "Constructors.h"
#include "List.h"
#include "Error.h"

#define TakeSeveralArguments(from, to, error) TakeArguments(from, to, sizeof(to)/sizeof(to[0]), sizeof(to)/sizeof(to[0]), error)
#define TakeSingleArgument(from, to, error) TakeArguments(from, to, 1, 1, error)

#define CheckTermType(term, terTag) if (terTag != term->tag) return InvalidArgumentType()
#define CheckErrorTerm(term) if (terError == term->tag) return term
#define EvalTermAndCheckError(variable, expression, bindings) if (terError == (variable = EvalRecursive((expression), (bindings)))->tag) return variable