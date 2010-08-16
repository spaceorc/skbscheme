#include <stdarg.h>

#include "Constructors.h"
#include "List.h"
#include "MemoryManager.h"

Term * Number(int number) {
	Term * result = AllocateTerm(terNumber);
	result->number = number;
	return result;
}

Term * Function(FunctionPtr function) {
	Term * result = AllocateTerm(terFunction);
	result->function = function;
	return result;
}

Term * Cons(Term * first, Term * second) {
	Term * result = 0;
	result = AllocateTerm(terPair);	
	result->pair = AllocatePair();
	result->pair->first = first;
	result->pair->second = second;
	return result;
}

Term * Error(LimitedStr str) {
	Term * result = AllocateTerm(terError);
	result->message = str;
	return result;
}

Term * Nil() {
	return AllocateTerm(terNil);
}

List MakeListFromArray(unsigned int count, Term * terms[]) {
	List result = 0;
	Pair * iterator = 0;
	unsigned int i = 0;
	while (i < count)
		result = AppendUsingIterator(result, &iterator, terms[i++]);
	return result;
}

List MakeList(unsigned int count, ...) {
	List result = 0;
	Pair * iterator = 0;
	unsigned int i = 0;
	va_list listPointer;
	va_start(listPointer, count);
	while(i++ < count)
		result = AppendUsingIterator(result, &iterator, va_arg(listPointer, Term *));
	return result;
}

Term * Redex(List redex) {
	Term * result = AllocateTerm(terRedex);
	result->redex = redex;
	return result;
}

Term * StringFromConstantStr(ConstantStr str) {
	return StringFromConstantLimitedStr(LimitConstantStr(str));
}

Term * StringFromConstantLimitedStr(ConstantLimitedStr str) {
	return String(LimitedStrFromConstantLimitedStr(str));
}

Term * String(LimitedStr str) {
	Term * result = AllocateTerm(terString);
	result->string = str;
	return result;
}

Term * Variable(LimitedStr str) {
	Term * result = AllocateTerm(terVariable);
	result->variable = str;
	return result;
}

Term * VariableFromConstantStr(ConstantStr str) {
	return VariableFromConstantLimitedStr(LimitConstantStr(str));
}

Term * VariableFromConstantLimitedStr(ConstantLimitedStr str) {
	return Variable(LimitedStrFromConstantLimitedStr(str));
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

Term * Void() {
	return AllocateTerm(terVoid);
}

Term * Character(Chr character) {
	Term * result = AllocateTerm(terCharacter);
	result->character = character;
	return result;
}

Term * FileDescriptor(int fildes) {
	Term * result = AllocateTerm(terFileDescriptor);
	result->fildes = fildes;
	return result;
}