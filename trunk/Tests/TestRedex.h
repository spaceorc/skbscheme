#include "Test.h"

TEST(GeneralEvalNotAFunction) {
	Term * a = AllocateTerm(terRedex);
	a->redex = MakeList(3, Number(0), Number(1), Number(2));
	AssertTag(terError, Eval(a, 0));
}

TEST(GeneralInternalApplyWith0Arguments) {
	Term * a = AllocateTerm(terRedex);
	a->redex = MakeList(0);
	AssertTag(terError, Eval(a, 0));
}

TEST(EvalScalar) {
	AssertEq(Number(10), Eval(Number(10), 0));
}

TEST(EvalRedex) {
	Term * a = AllocateTerm(terRedex);
	a->redex = MakeList(3, Function(OperatorPlus), Number(1), Number(2));
	AssertEq(Number(3), Eval(a, 0));
}

TEST(EvalRecursiveRedex) {
	Term * a = AllocateTerm(terRedex);
	a->redex = MakeList(3, Function(OperatorPlus), Number(1), Number(2));
	Term * b = AllocateTerm(terRedex);
	b->redex = MakeList(3, Function(OperatorMinus), Number(1), a);
	AssertEq(Number(-2), Eval(b, 0));
}

Term * z_a;

Term * MockContextBoundFunction(ContextBindings * contextBindings) {
	z_a = InternalFind(contextBindings->dictionary, LimitConstStr("z_a"));
	return Number(2);
}

TEST(EvalContextBoundFunction) {
	z_a = 0;
	Term * a = AllocateTerm(terRedex);
	a->redex = MakeList(2, ContextFunction(MockContextBoundFunction, MakeList(1, ConstantString("z_a"))), Number(1));
	ContextBindings contextBindings;
	contextBindings.dictionary = 0;
	contextBindings.previous = 0;
	AssertEq(Number(2), Eval(a, &contextBindings));
	AssertEq(Number(1), z_a);
}
