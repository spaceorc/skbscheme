#include "Test.h"
#include "Evaluation.h"
#include <memory.h>
#include <malloc.h>

int expectedEvaluateCallCount = 0;
Term * expectedEvaluateResult = 0;
int childEvaluatedCallCount = 0;

EvaluationContextBase * MockEvaluate(EvaluationContextBase * evaluationContext) {
	AssertThat(expectedEvaluateCallCount-- >= 0);
	if (expectedEvaluateCallCount == 0)
		evaluationContext->result = expectedEvaluateResult;
	return evaluationContext;
}

EvaluationContextBase * MockChildEvaluated(EvaluationContextBase * evaluationContext, Term * childResult) {
	childEvaluatedCallCount++;
	evaluationContext->result = childResult;
	return evaluationContext;
}

static EvaluationContextBase * MockContext() {
	expectedEvaluateCallCount = 0;
	expectedEvaluateResult = 0;
	EvaluationContextBase * result = (EvaluationContextBase*) malloc(sizeof(*result));
	memset(result, 0, sizeof(*result));
	result->evaluate = MockEvaluate;
	result->childEvaluated = MockChildEvaluated;
	return result;
}

TEST(EvaluateAlreadyEvaluated) {
	EvaluationContextBase * context = MockContext();
	context->result = Number(10);
	Term * z = Evaluate(context);
	AssertEq(Number(10), z);
}

TEST(EvaluateNotEvaluated) {
	EvaluationContextBase * context = MockContext();
	expectedEvaluateCallCount = 1;
	expectedEvaluateResult = Number(100);
	Term * z = Evaluate(context);
	AssertEq(Number(100), z);
}

TEST(EvaluateCyclic) {
	EvaluationContextBase * context = MockContext();
	expectedEvaluateCallCount = 10;
	expectedEvaluateResult = Number(200);
	Term * z = Evaluate(context);
	AssertEq(Number(200), z);
}

TEST(EvaluateParent) {
	EvaluationContextBase * childContext = MockContext();
	EvaluationContextBase * parentContext = MockContext();
	childContext->result = Number(300);
	childContext->parent = parentContext;
	Term * z = Evaluate(childContext);
	AssertEq(Number(300), z);
	AssertThat(1 == childEvaluatedCallCount);
}
