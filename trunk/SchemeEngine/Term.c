#include "Term.h"
#include "MemoryManager.h"

Term * Function(FunctionPtr function) {
	Term * result = AllocateTerm(terFunction);
	result->function = function;
	return result;
}

Term * LazyFunction(LazyFunctionPtr lazyFunction, AcquireLazyEvaluationContextPtr acquireLazyEvaluationContext) {
	Term * result = AllocateTerm(terLazyFunction);
	result->lazy.function = lazyFunction;
	result->lazy.acquireEvaluationContext = acquireLazyEvaluationContext;
	return result;
}

Term * MakeLambda(List formalArguments, List body, ContextBindings * context) {
	Term * result = AllocateTerm(terLambda);
	result->lambda.formalArguments = formalArguments;
	result->lambda.body = body;
	result->lambda.context = context;
	return result;
}