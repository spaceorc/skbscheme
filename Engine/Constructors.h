#pragma once

#include "Term.h"

Term * Number(int number);
Term * Function(FunctionPtr function);
Term * InternalCons(Term * first, Term * second);
// Error
Term * Nil();
// Redex
Term * ConstantStringFromConstantStr(ConstantStr str);
Term * ConstantStringFromConstantLimitedStr(ConstantLimitedStr str);
Term * ConstantString(LimitedStr str);
Term * Symbol(LimitedStr str);
Term * SymbolFromConstantStr(ConstantStr str);
Term * SymbolFromConstantLimitedStr(ConstantLimitedStr str);
Term * LazyFunction(LazyFunctionPtr lazyFunction);
Term * DefineFunction(List formalArguments, Term * function);
