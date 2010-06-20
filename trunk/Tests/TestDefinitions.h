#include "Test.h"

Term * MockFunction(List arguments) {
	Term * arg = 0, * error = 0;
	AssertThat(TakeSingleArgument(arguments, &arg, &error) >= 0);
	AssertTag(terNumber, arg);
	return Number(arg->number + 1);
}

TEST(LetSingle) {
	ContextBindings * contextBindings = AcquireContextBindings();
	InternalSetConstantStr(contextBindings->dictionary, STR("lalala"), Function(MockFunction));
	Term * a = ParseSingle(STR("((bububu 1))"));
	Term * b = ParseSingle(STR("(lalala bububu)"));
	Term * z = MakeRedex(3, LazyFunction(LazyFunctionLet), a, b);
	AssertEq(Number(2), Eval(z, contextBindings));
	AssertThat(0 == InternalFindConstantStr(contextBindings->dictionary, STR("bububu")));
	AssertEq(Function(MockFunction), InternalFindConstantStr(contextBindings->dictionary, STR("lalala")));
}

TEST(LetMultiple) {
	ContextBindings * contextBindings = AcquireContextBindings();
	InternalSetConstantStr(contextBindings->dictionary, STR("lalala"), Function(MockFunction));
	Term * a = ParseSingle(STR("((bububu 1) (kukuku (lalala bububu)))"));	
	// todo error here should be: bububu is not defined 
	Term * b = ParseSingle(STR("(lalala kukuku)"));
	Term * z = MakeRedex(3, LazyFunction(LazyFunctionLet), a, b);
	AssertEq(Number(3), Eval(z, contextBindings));
	AssertThat(0 == InternalFindConstantStr(contextBindings->dictionary, STR("bububu")));
	AssertThat(0 == InternalFindConstantStr(contextBindings->dictionary, STR("kukuku")));
	AssertEq(Function(MockFunction), InternalFindConstantStr(contextBindings->dictionary, STR("lalala")));
}

TEST(DefineVariable) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = MakeRedex(3, LazyFunction(LazyFunctionDefine), SymbolFromConstantStr(STR("lalala")), Number(10));
	/* ??? AssertEq(Nil(), ??? */Eval(z, contextBindings)/* ??? ) ??? */;
	AssertEq(Number(10), InternalFindConstantStr(contextBindings->dictionary, STR("lalala")));
}

TEST(DefineVariableEvaluatesValue) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = MakeRedex(3, LazyFunction(LazyFunctionDefine), SymbolFromConstantStr(STR("lalala")), MakeRedex(3, Function(OperatorPlus), Number(10), Number(20)));
	/* ??? AssertEq(Nil(), ??? */Eval(z, contextBindings)/* ??? ) ??? */;
	AssertEq(Number(30), InternalFindConstantStr(contextBindings->dictionary, STR("lalala")));
}

TEST(DefineFunction) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = MakeRedex(3, LazyFunction(LazyFunctionDefine), ParseSingle(STR("(func p1 p2)")), ParseSingle(STR("(+ p1 p2)")));
	/* ??? AssertEq(Nil(), ??? */Eval(z, contextBindings)/* ??? ) ??? */;
    Term * func = InternalFindConstantStr(contextBindings->dictionary, STR("func"));
	AssertTag(terDefinedFunction, func);
	AssertEq(ParseSingle(STR("(+ p1 p2)")), func->definedFunction.function);
	AssertEq(SymbolFromConstantStr(STR("p1")), IterateList(&func->definedFunction.formalArguments));
	AssertEq(SymbolFromConstantStr(STR("p2")), IterateList(&func->definedFunction.formalArguments));
	AssertThat(0 == IterateList(&func->definedFunction.formalArguments));
}