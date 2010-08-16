#include "Term.h"
#include "Error.h"

Term * Nil();
List MakeListFromArray(unsigned int count, Term * terms[]);
List MakeList(unsigned int count, ...);Term * Iterate(List * iterator);
List AppendUsingIterator(List list, Pair ** iterator, Term * term);
List Append(List list, Term * term);
List Push(List list, Term * term);
int TakeArguments(List from, Term * to[], int atLeast, int atMost, Term ** error);

#define TakeSeveralArguments(from, to, error) TakeArguments(from, to, sizeof(to)/sizeof(to[0]), sizeof(to)/sizeof(to[0]), error)
#define TakeSingleArgument(from, to, error) TakeArguments(from, to, 1, 1, error)

#define CheckTermType(term, terTag) if (terTag != term->tag) return InvalidArgumentType()
#define CheckErrorTerm(term) if (terError == term->tag) return term
#define EvalTermAndCheckError(variable, expression, bindings) if (terError == (variable = EvalRecursive((expression), (bindings)))->tag) return variable