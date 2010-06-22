#include "Constructors.h"
#include "Memory.h"

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

Term * InternalCons(Term * first, Term * second) {
	Term * result = 0;
	result = AllocateTerm(terPair);	
	result->pair = AllocatePair();
	result->pair->first = first;
	result->pair->second = second;
	return result;
}

Term * Nil() {
	return AllocateTerm(terNil);
}

Term * ConstantStringFromConstantStr(ConstantStr str) {
	return ConstantStringFromConstantLimitedStr(LimitConstantStr(str));
}

Term * ConstantStringFromConstantLimitedStr(ConstantLimitedStr str) {
	return ConstantString(LimitedStrFromConstantLimitedStr(str));
}

Term * ConstantString(LimitedStr str) {
	Term * result = AllocateTerm(terConstantString);
	result->constantString = str;
	return result;
}

Term * Symbol(LimitedStr str) {
	Term * result = AllocateTerm(terSymbol);
	result->symbol = str;
	return result;
}

Term * SymbolFromConstantStr(ConstantStr str) {
	return SymbolFromConstantLimitedStr(LimitConstantStr(str));
}

Term * SymbolFromConstantLimitedStr(ConstantLimitedStr str) {
	return Symbol(LimitedStrFromConstantLimitedStr(str));
}

Term * LazyFunction(LazyFunctionPtr lazyFunction) {
	Term * result = AllocateTerm(terLazyFunction);
	result->lazyFunction = lazyFunction;
	return result;
}

Term * DefineFunction(List formalArguments, Term * function, ContextBindings * context, int useRuntimeContext) {
	Term * result = AllocateTerm(terDefinedFunction);
	result->definedFunction.formalArguments = formalArguments;
	result->definedFunction.function = function;
	result->definedFunction.context = context;
	result->definedFunction.useRuntimeContext = useRuntimeContext;
	return result;
}

Term * Empty() {
	return AllocateTerm(terEmpty);
}