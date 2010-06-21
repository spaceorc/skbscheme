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
	InternalSetConstantStr(contextBindings->dictionary, STR("lalala"), DefineFunction(MakeList(2, SymbolFromConstantStr(STR("a")), SymbolFromConstantStr(STR("b"))), MakeRedex(3, Function(OperatorPlus), SymbolFromConstantStr(STR("a")), SymbolFromConstantStr(STR("b")))));
	AssertEq(Number(3), Eval(MakeRedex(3, SymbolFromConstantStr(STR("lalala")), Number(1), Number(2)), contextBindings));
	AssertThat(0 == InternalFindConstantStr(contextBindings->dictionary, STR("a")));
	AssertThat(0 == InternalFindConstantStr(contextBindings->dictionary, STR("b")));
}

TEST(EvalDefinedFunctionInsufficientAgruments) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle(STR("(define (lalala p1 p2) (+ p1 p2))"));
	/* ??? AssertEq(Nil(), ??? */Eval(z, contextBindings)/* ??? ) ??? */;
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle(STR("(lalala 10)")), contextBindings));
}

TEST(EvalDefinedFunctionTooManyAgruments) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle(STR("(define (lalala p1 p2) (+ p1 p2))"));
	/* ??? AssertEq(Nil(), ??? */Eval(z, contextBindings)/* ??? ) ??? */;
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle(STR("(lalala 10 20 30)")), contextBindings));
}

TEST(EvalDefinedFunctionBadArgumentsList) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle(STR("(define (lalala p1 (p2 p3)) (+ p1 p2))"));
	/* ??? AssertEq(Nil(), ??? */Eval(z, contextBindings)/* ??? ) ??? */;
	AssertEq(InvalidArgumentType(), Eval(ParseSingle(STR("(lalala 10 20 30)")), contextBindings));
}

TEST(EvalDefinedFunctionEvaluatesArgumentsFirst) {
	ContextBindings * contextBindings = AcquireContextBindings();
	InternalSetConstantStr(contextBindings->dictionary, STR("lalala"), 
		DefineFunction(
			MakeList(3, SymbolFromConstantStr(STR("a")), SymbolFromConstantStr(STR("b")), SymbolFromConstantStr(STR("c"))), 
			MakeRedex(3, Function(OperatorPlus), SymbolFromConstantStr(STR("a")), SymbolFromConstantStr(STR("b")))));
	AssertEq(InvalidArgumentType(), Eval(MakeRedex(4, SymbolFromConstantStr(STR("lalala")), Number(1), Number(2), InvalidArgumentType()), contextBindings));
}
