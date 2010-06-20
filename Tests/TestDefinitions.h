#include "Test.h"

Term * MockFunction(List arguments) {
	Term * arg = 0, * error = 0;
	AssertThat(TakeSingleArgument(arguments, &arg, &error) >= 0);
	AssertTag(terNumber, arg);
	return Number(arg->number + 1);
}

TEST(LetSingle) {
	ContextBindings * contextBindings = AcquireContextBindings();
	InternalSet(contextBindings->dictionary, "lalala", Function(MockFunction));
	Term * a = ParseSingle("((bububu 1))");	
	Term * b = ParseSingle("(lalala bububu)");
	Term * z = MakeRedex(3, LazyFunction(LazyFunctionLet), a, b);
	AssertEq(Number(2), Eval(z, contextBindings));
	AssertThat(0 == InternalFind(contextBindings->dictionary, LimitConstStr("bububu")));
	AssertEq(Function(MockFunction), InternalFind(contextBindings->dictionary, LimitConstStr("lalala")));
}

TEST(LetMultiple) {
	ContextBindings * contextBindings = AcquireContextBindings();
	InternalSet(contextBindings->dictionary, "lalala", Function(MockFunction));
	Term * a = ParseSingle("((bububu 1) (kukuku (lalala bububu)))");	
	// todo error here should be: bububu is not defined 
	Term * b = ParseSingle("(lalala kukuku)");
	Term * z = MakeRedex(3, LazyFunction(LazyFunctionLet), a, b);
	AssertEq(Number(3), Eval(z, contextBindings));
	AssertThat(0 == InternalFind(contextBindings->dictionary, LimitConstStr("bububu")));
	AssertThat(0 == InternalFind(contextBindings->dictionary, LimitConstStr("kukuku")));
	AssertEq(Function(MockFunction), InternalFind(contextBindings->dictionary, LimitConstStr("lalala")));
}

TEST(DefineVariable) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = MakeRedex(3, LazyFunction(LazyFunctionDefine), Symbol("lalala"), Number(10));
	/* ??? AssertEq(Nil(), ??? */Eval(z, contextBindings)/* ??? ) ??? */;
	AssertEq(Number(10), InternalFind(contextBindings->dictionary, LimitConstStr("lalala")));
}

TEST(DefineVariableEvaluatesValue) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = MakeRedex(3, LazyFunction(LazyFunctionDefine), Symbol("lalala"), MakeRedex(3, Function(OperatorPlus), Number(10), Number(20)));
	/* ??? AssertEq(Nil(), ??? */Eval(z, contextBindings)/* ??? ) ??? */;
	AssertEq(Number(30), InternalFind(contextBindings->dictionary, LimitConstStr("lalala")));
}

TEST(DefineFunction) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = MakeRedex(3, LazyFunction(LazyFunctionDefine), ParseSingle("(func p1 p2)"), ParseSingle("(+ p1 p2)"));
	/* ??? AssertEq(Nil(), ??? */Eval(z, contextBindings)/* ??? ) ??? */;
    Term * func = InternalFind(contextBindings->dictionary, LimitConstStr("func"));
	AssertTag(terDefinedFunction, func);
	AssertEq(ParseSingle("(+ p1 p2)"), func->definedFunction.function);
	AssertEq(Symbol("p1"), IterateList(&func->definedFunction.formalArguments));
	AssertEq(Symbol("p2"), IterateList(&func->definedFunction.formalArguments));
	AssertThat(0 == IterateList(&func->definedFunction.formalArguments));
}