#include "Term.h"

Term * Iterate(List * iterator);
List AppendUsingIterator(List list, Pair ** iterator, Term * term);
List Append(List list, Term * term);
List Push(List list, Term * term);
int TakeArguments(List from, Term * to[], int atLeast, int atMost, Term ** error);