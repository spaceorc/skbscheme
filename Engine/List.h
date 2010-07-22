#include "Term.h"

Term * IterateList(List * iterator);
List AppendListUsingIterator(List list, Pair ** iterator, Term * term);
List AppendList(List list, Term * term);
List PushList(List list, Term * term);
int TakeArguments(List from, Term * to[], int atLeast, int atMost, Term ** error);