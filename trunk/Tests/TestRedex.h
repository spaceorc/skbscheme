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