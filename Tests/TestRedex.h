#include "Test.h"

TEST(GeneralInternalApply) {
	Term * z = FunctionInternalApply(MakeList(3, Function(OperatorPlus), Number(1), Number(2)));
	AssertEq(Number(3), z);
}

TEST(GeneralInternalApplyNotAFunction) {
	Term * z = FunctionInternalApply(MakeList(3, Number(0), Number(1), Number(2)));
	AssertTag(terError, z);
}

TEST(GeneralInternalApplyWith0Arguments) {
	Term * z = FunctionInternalApply(MakeList(0));
	AssertTag(terError, z);
}

TEST(EvalScalar) {
	Term * z = Eval(Number(10));
	AssertEq(Number(10), z);
}

TEST(EvalRedex) {
	Term * a = AllocateTerm(terRedex);
	a->redex = MakeList(3, Function(OperatorPlus), Number(1), Number(2));
	Term * z = Eval(a);
	AssertEq(Number(3), z);
}

TEST(EvalRecursiveRedex) {
	Term * a = AllocateTerm(terRedex);
	a->redex = MakeList(3, Function(OperatorPlus), Number(1), Number(2));
	Term * b = AllocateTerm(terRedex);
	b->redex = MakeList(3, Function(OperatorMinus), Number(1), a);
	Term * z = Eval(b);
	AssertEq(Number(-2), z);
}