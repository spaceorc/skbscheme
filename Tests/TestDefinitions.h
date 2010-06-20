#include "Test.h"

Term * MockFunction(List arguments) {
	Term * arg = 0, * error = 0;
	AssertThat(TakeSingleArgument(arguments, &arg, &error) >= 0);
	AssertEq(Number(1), arg);
	return Number(2);
}

TEST(TestLet) {
	ContextBindings * contextBindings = AcquireContextBindings();
	InternalSet(contextBindings->dictionary, "lalala", Function(MockFunction));
	Term * a = AllocateTerm(terRedex);
	a->redex = MakeList(2, Symbol("bububu"), Number(1));
	Term * b = AllocateTerm(terRedex);
	b->redex = MakeList(1, a);
	Term * c = AllocateTerm(terRedex);
	c->redex = MakeList(2, Symbol("lalala"), Symbol("bububu"));
	Term * d = AllocateTerm(terRedex);
	d->redex = MakeList(3, LazyFunction(LazyFunctionLet), b, c);
	AssertEq(Number(2), Eval(c, contextBindings));
	AssertThat(0 == InternalFind(contextBindings->dictionary, LimitConstStr("bububu")));
	AssertEq(Function(MockFunction), InternalFind(contextBindings->dictionary, LimitConstStr("lalala")));
}