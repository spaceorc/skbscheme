#include "Test.h"
#include <memory.h>
#include <malloc.h>

TEST(GeneralEvalNotAFunction) {
	Term * a = AllocateTerm(terRedex);
	a->redex = MakeList(3, Number(0), Number(1), Number(2));
	AssertTag(terError, Eval(a, 0));
}

TEST(GeneralEvalWith0Arguments) {
	Term * a = AllocateTerm(terRedex);
	a->redex = MakeList(0);
	AssertTag(terError, Eval(a, 0));
}

TEST(EvalScalar) {
	AssertEq(Number(10), Eval(ParseSingle("10"), 0));
}

TEST(EvalRedex) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(3), Eval(ParseSingle("(+ 1 2)"), contextBindings));
}

TEST(EvalRecursiveRedex) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(-2), Eval(ParseSingle("(- 1 (+ 1 2))"), contextBindings));
}

static Term * MockLazyFunction(List arguments, ContextBindings * contextBindings) {
	AssertTag(terRedex, IterateList(&arguments));
	AssertThat(0 == IterateList(&arguments));
	return Number(5);
}

static EvaluationContextBase * MockChildEvaluated1(EvaluationContextBase * evaluationContext, Term * childResult) {
	assert(0);
	return 0;
}

struct MockLazyEvaluationContext {
	EvaluationContextBase base;
	List arguments;
};

static EvaluationContextBase * MockEvaluate1(MockLazyEvaluationContext * evaluationContext) {
	AssertTag(terRedex, IterateList(&evaluationContext->arguments));
	AssertThat(0 == IterateList(&evaluationContext->arguments));
	THIS_CONTEXT->result = Number(5);
	return THIS_CONTEXT;
}

static EvaluationContextBase * MockAcquireLazyEvaluationContext(EvaluationContextBase * parent, ContextBindings * contextBindings, List arguments) {
	MockLazyEvaluationContext * result = (MockLazyEvaluationContext *) malloc(sizeof(*result));
	FillEvaluationContextBase((EvaluationContextBase *) result, parent, contextBindings, MockChildEvaluated1, (EvaluatePtr) MockEvaluate1);
	result->arguments = arguments;
	return (EvaluationContextBase *) result;
}

TEST(EvalRedexStartingWithLazyFunction) {
	ContextBindings * contextBindings = AcquireContextBindings();
	contextBindings->dictionary = SetConstantStr(contextBindings->dictionary, STR("lalala"), LazyFunction(MockLazyFunction, MockAcquireLazyEvaluationContext));
	AssertEq(Number(5), Eval(ParseSingle("(lalala (+ 1 2))"), contextBindings));
}

TEST(EvalRedexStartingWithDefinedFunction) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle("(define (lalala p1 p2) (+ p1 p2))");
	AssertEq(Void(), Eval(z, contextBindings));
	AssertEq(Number(3), Eval(ParseSingle("(lalala 1 2)"), contextBindings));
	AssertThat(0 == FindConstantStr(contextBindings->dictionary, STR("a")));
	AssertThat(0 == FindConstantStr(contextBindings->dictionary, STR("b")));
}

TEST(EvalDefinedFunctionInsufficientAgruments) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle("(define (lalala p1 p2) (+ p1 p2))");
	AssertEq(Void(), Eval(z, contextBindings));
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(lalala 10)"), contextBindings));
}

TEST(EvalDefinedFunctionTooManyAgruments) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle("(define (lalala p1 p2) (+ p1 p2))");
	AssertEq(Void(), Eval(z, contextBindings));
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(lalala 10 20 30)"), contextBindings));
}

TEST(EvalDefinedFunctionBadArgumentsList) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle("(define (lalala p1 (p2 p3)) (+ p1 p2))");
	AssertEq(Void(), Eval(z, contextBindings));
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(lalala 10 20 30)"), contextBindings));
}

TEST(EvalDefinedFunctionEvaluatesArgumentsFirst) {
	ContextBindings * contextBindings = AcquireContextBindings();
	Term * z = ParseSingle("(define (lalala p1 p2 p3) (+ p1 p2))");
	AssertEq(Void(), Eval(z, contextBindings));
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(lalala 1 2 (+ 1))"), contextBindings));
}

TEST(EvalDefinedFunctionClosedToItsContext) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Void(), Eval(ParseSingle("(define (lalala a b) (+ a b c))"), contextBindings));
	AssertEq(Void(), Eval(ParseSingle("(define c 10)"), contextBindings));
	AssertEq(Number(13), Eval(ParseSingle("(let ((c 100)) (lalala 1 2))"), contextBindings));
}

TEST(EvalDefinesNestedDefineInsideOuterDefineScope) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Void(), Eval(ParseSingle("(define (lalala p1 p2) (define (bububu p3) (+ p1 p3)) (bububu p2))"), contextBindings));
	AssertEq(Number(3), Eval(ParseSingle("(lalala 1 2)"), contextBindings));
	AssertThat(0 == Find(contextBindings->dictionary, LimitedStrFromConstantStr(STR("bububu"))));
}

TEST(EvalResolvedVariablesUsingOutermostRelevantScope) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Void(), Eval(ParseSingle("(define (lalala p1 p2) (define (bububu p3) (+ p1 p3)) (let((p1 10)) (bububu p2)))"), contextBindings));
	AssertEq(Number(3), Eval(ParseSingle("(lalala 1 2)"), contextBindings));
}