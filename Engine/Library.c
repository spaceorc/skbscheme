#include <assert.h>

#include "Library.h"
#include "Number.h"
#include "Pair.h"
#include "Error.h"
#include "FileDescriptor.h"
#include "Definitions.h"
#include "Boolean.h"
#include "Dictionary.h"
#include "LetEvaluation.h"
#include "DefineEvaluation.h"
#include "BooleanEvaluation.h"

ConstantStr globalFunctionNames [] = {"+", "-", "=", "cons", "car", "cdr", "error", "posix-open", "posix-close", "posix-read", "posix-write-string", "posix-write-term"};
FunctionPtr globalFunctionPointers [] = {OperatorPlus, OperatorMinus, OperatorNumberEq, FunctionCons, FunctionCar, FunctionCdr, FunctionError, FunctionOpen, FunctionClose, FunctionRead, FunctionWrite, FunctionWriteTerm};
ConstantStr globalLazyFunctionNames [] = {"let", "define", "lambda", "and", "or", "if", "cond"};
LazyFunctionPtr globalLazyFunctionPointers [] = {LazyFunctionLet, LazyFunctionDefine, LazyFunctionLambda, LazyFunctionAnd, LazyFunctionOr, LazyFunctionIf, LazyFunctionCond};
AcquireLazyEvaluationContextPtr globalAcquireLazyEvaluationContextPointers [] = {AcquireLetEvaluationContext, AcquireDefineEvaluationContext, AcquireDefineLambdaEvaluationContext, AcquireEmptyLazyEvaluationContext, AcquireEmptyLazyEvaluationContext, AcquireEmptyLazyEvaluationContext, AcquireEmptyLazyEvaluationContext};
ConstantStr globalConstantNames [] = {"#t", "#f", "null", "else", "posix-stdin", "posix-stdout", "posix-stderr"};
CreateConstantPtr globalConstantFunctionPointers [] = {True, False, Nil, True, StdIn, StdOut, StdErr};

ContextBindings * AcquireContextBindings() {
	ContextBindings * result = AllocateContextBindings(0);
	int lenFunctions = sizeof(globalFunctionNames)/sizeof(globalFunctionNames[0]);
	int lenLazyFunctions = sizeof(globalLazyFunctionNames)/sizeof(globalLazyFunctionNames[0]);
	int lenConstants = sizeof(globalConstantNames)/sizeof(globalConstantNames[0]);
	assert((sizeof(globalFunctionPointers)/sizeof(globalFunctionPointers[0])) == lenFunctions);
	while (lenFunctions-- > 0)
		result->dictionary = InternalSetConstantStr(result->dictionary, globalFunctionNames[lenFunctions], Function(globalFunctionPointers[lenFunctions]));
	assert((sizeof(globalLazyFunctionPointers)/sizeof(globalLazyFunctionPointers[0])) == lenLazyFunctions);
	assert((sizeof(globalAcquireLazyEvaluationContextPointers)/sizeof(globalAcquireLazyEvaluationContextPointers[0])) == lenLazyFunctions);
	while (lenLazyFunctions-- > 0)
		result->dictionary = InternalSetConstantStr(result->dictionary, globalLazyFunctionNames[lenLazyFunctions], LazyFunction(globalLazyFunctionPointers[lenLazyFunctions], globalAcquireLazyEvaluationContextPointers[lenLazyFunctions]));	
	assert((sizeof(globalConstantFunctionPointers)/sizeof(globalConstantFunctionPointers[0])) == lenConstants);
	while (lenConstants-- > 0)
		result->dictionary = InternalSetConstantStr(result->dictionary, globalConstantNames[lenConstants], (globalConstantFunctionPointers[lenConstants])());
	return result;
}
