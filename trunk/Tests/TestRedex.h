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

TEST(EvalRedexUnresolvedSymbols) {
	Term * a = AllocateTerm(terRedex);
	a->redex = MakeList(3, SymbolFromConstantStr(STR("+")), Number(1), Number(2));
	AssertEq(Number(3), Eval(a, AcquireContextBindings()));
}

Term * MockLazyFunction(List arguments, ContextBindings * contextBindings) {
	AssertTag(terRedex, IterateList(&arguments));
	AssertThat(0 == IterateList(&arguments));
	return Number(5);
}

TEST(EvalRedexStartingWithLazyFunction) {
	ContextBindings * contextBindings = AcquireContextBindings();
	InternalSetConstantStr(contextBindings->dictionary, STR("lalala"), LazyFunction(MockLazyFunction));
	Term * a = MakeRedex(3, Function(OperatorPlus), Number(1), Number(2));
	Term * b = MakeRedex(2, SymbolFromConstantStr(STR("lalala")), a);
	AssertEq(Number(5), Eval(b, contextBindings));
}

TEST(EvalRedexStartingWithDefinedFunction) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle("(define (lalala p1 p2) (+ p1 p2))");
	AssertEq(Empty(), Eval(z, contextBindings));
	AssertEq(Number(3), Eval(ParseSingle("(lalala 1 2)"), contextBindings));
	AssertThat(0 == InternalFindConstantStr(contextBindings->dictionary, STR("a")));
	AssertThat(0 == InternalFindConstantStr(contextBindings->dictionary, STR("b")));
}

TEST(EvalDefinedFunctionInsufficientAgruments) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle("(define (lalala p1 p2) (+ p1 p2))");
	AssertEq(Empty(), Eval(z, contextBindings));
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(lalala 10)"), contextBindings));
}

TEST(EvalDefinedFunctionTooManyAgruments) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle("(define (lalala p1 p2) (+ p1 p2))");
	AssertEq(Empty(), Eval(z, contextBindings));
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(lalala 10 20 30)"), contextBindings));
}

TEST(EvalDefinedFunctionBadArgumentsList) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle("(define (lalala p1 (p2 p3)) (+ p1 p2))");
	AssertEq(Empty(), Eval(z, contextBindings));
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(lalala 10 20 30)"), contextBindings));
}

TEST(EvalDefinedFunctionEvaluatesArgumentsFirst) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle("(define (lalala p1 p2 p3) (+ p1 p2))");
	AssertEq(Empty(), Eval(z, contextBindings));
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(lalala 1 2 (+ 1))"), contextBindings));
}

TEST(EvalDefinedFunctionClosedToItsContext) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Empty(), Eval(ParseSingle("(define (lalala a b) (+ a b c))"), contextBindings));
	InternalSetConstantStr(contextBindings->dictionary, STR("c"), Number(10));
	AssertEq(Number(13), Eval(ParseSingle("(let ((c 100)) (lalala 1 2))"), contextBindings));
}