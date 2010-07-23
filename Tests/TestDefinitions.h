#include "Test.h"

Term * MockFunction(List arguments) {
	Term * arg = 0, * error = 0;
	AssertThat(TakeSingleArgument(arguments, &arg, &error) >= 0);
	AssertTag(terNumber, arg);
	return Number(arg->number + 1);
}

TEST(LetSingle) {
	ContextBindings * contextBindings = AcquireContextBindings();
	contextBindings->dictionary = SetConstantStr(contextBindings->dictionary, STR("lalala"), Function(MockFunction));
	Term * a = ParseSingle("((bububu 1))");
	Term * b = ParseSingle("(lalala bububu)");
	Term * z = Redex(MakeList(3, LazyFunction(LazyFunctionLet, AcquireLetEvaluationContext), a, b));
	AssertEq(Number(2), Eval(z, contextBindings));
	AssertThat(0 == FindConstantStr(contextBindings->dictionary, STR("bububu")));
	AssertEq(Function(MockFunction), FindConstantStr(contextBindings->dictionary, STR("lalala")));
}

TEST(LetMultipleExpressions) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(3), Eval(ParseSingle("(let () 1 2 3)"), contextBindings));
}

TEST(LetNoExpressions) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(let ((a 10)) )"), contextBindings));
}

TEST(LetInvalid) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(let a a)"), contextBindings));
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(let (a 10) a)"), contextBindings));
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(let ((a)) a)"), contextBindings));
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(let ((10 20)) a)"), contextBindings));
}

TEST(LetMultiple) {
	ContextBindings * contextBindings = AcquireContextBindings();
	contextBindings->dictionary = SetConstantStr(contextBindings->dictionary, STR("lalala"), Function(MockFunction));
	Term * a = ParseSingle("((bububu 10) (kukuku (lalala 2)))");
	Term * b = ParseSingle("(+ bububu (lalala kukuku))");
	Term * z = Redex(MakeList(3, LazyFunction(LazyFunctionLet, AcquireLetEvaluationContext), a, b));
	AssertEq(Number(14), Eval(z, contextBindings));
	AssertThat(0 == FindConstantStr(contextBindings->dictionary, STR("bububu")));
	AssertThat(0 == FindConstantStr(contextBindings->dictionary, STR("kukuku")));
	AssertEq(Function(MockFunction), FindConstantStr(contextBindings->dictionary, STR("lalala")));
}

TEST(LetEvalsVariablesFirst) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(let ((a (+ 1))) 10)"), contextBindings));
}

TEST(DefineVariable) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = Redex(MakeList(3, LazyFunction(LazyFunctionDefine, AcquireDefineEvaluationContext), VariableFromConstantStr(STR("lalala")), Number(10)));
	AssertEq(Void(), Eval(z, contextBindings));
	AssertEq(Number(10), FindConstantStr(contextBindings->dictionary, STR("lalala")));
}

TEST(DefineVariableEvaluatesValue) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = Redex(MakeList(3, LazyFunction(LazyFunctionDefine, AcquireDefineEvaluationContext), VariableFromConstantStr(STR("lalala")), Redex(MakeList(3, Function(OperatorPlus), Number(10), Number(20)))));
	AssertEq(Void(), Eval(z, contextBindings));
	AssertEq(Number(30), FindConstantStr(contextBindings->dictionary, STR("lalala")));
}

TEST(DefineFunction) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = Redex(MakeList(3, LazyFunction(LazyFunctionDefine, AcquireDefineEvaluationContext), ParseSingle("(func p1 p2)"), ParseSingle("(+ p1 p2)")));
	AssertEq(Void(), Eval(z, contextBindings));
    Term * func = FindConstantStr(contextBindings->dictionary, STR("func"));
	AssertTag(terLambda, func);
	AssertEq(ParseSingle("(+ p1 p2)"), Iterate(&func->lambda.body));
	AssertThat(0 == Iterate(&func->lambda.body));
	AssertEq(VariableFromConstantStr(STR("p1")), Iterate(&func->lambda.formalArguments));
	AssertEq(VariableFromConstantStr(STR("p2")), Iterate(&func->lambda.formalArguments));
	AssertThat(0 == Iterate(&func->lambda.formalArguments));
}

TEST(DefineWithZeroArguments) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(define)"), contextBindings));
}

TEST(DefineWithNumberAsName) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = Redex(MakeList(3, LazyFunction(LazyFunctionDefine, AcquireDefineEvaluationContext), ParseSingle("10"), ParseSingle("(+ p1 p2)")));
	AssertEq(InvalidArgumentType(), Eval(z, contextBindings));
}

TEST(DefineWithNumberAsFunctionName) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = Redex(MakeList(3, LazyFunction(LazyFunctionDefine, AcquireDefineEvaluationContext), ParseSingle("(10 p1 p2)"), ParseSingle("(+ p1 p2)")));
	AssertEq(InvalidArgumentType(), Eval(z, contextBindings));
}

TEST(LambdaWithOneArgument) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle("((lambda(x) (+ x 2)) 3)");
	AssertEq(Number(5), Eval(z, contextBindings));
}

TEST(LambdaWithInvalidArgumentNames) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle("(lambda 1 (+ x 2))");
	AssertEq(InvalidArgumentType(), Eval(z, contextBindings));
}

TEST(LambdaWithMultipleExpressions) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle("((lambda () 1 2 3))");
	AssertEq(Number(3), Eval(z, contextBindings));
}