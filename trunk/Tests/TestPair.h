#include "Test.h"

TEST(PairCons) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Cons(Number(1), Number(2)), Eval(ParseSingle("(cons 1 2)"), contextBindings));
}

TEST(PairConsWithOneArgument) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(cons 1)"), contextBindings));
}

TEST(PairConsWithThreeArguments) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(cons 1 2 3)"), contextBindings));
}

TEST(PairConsWithErrorInside) {
	Term * z = FunctionCons(MakeList(2, AllocateTerm(terError), Number(1)));
    AssertTag(terError, z);
}

TEST(PairCar) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(1), Eval(ParseSingle("(car (cons 1 2))"), contextBindings));
}

TEST(PairCarEmptyList) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(ContractError(), Eval(ParseSingle("(car null)"), contextBindings));
}

TEST(PairCdr) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(2), Eval(ParseSingle("(cdr (cons 1 2))"), contextBindings));
}

TEST(PairCdrEmptyList) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(ContractError(), Eval(ParseSingle("(cdr null)"), contextBindings));
}