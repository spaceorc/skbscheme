#pragma once

#include "Term.h"

Term * Number(int number);
Term * Function(FunctionPtr function);
Term * InternalCons(Term * first, Term * second);
Term * Error(LimitedStr str);
Term * Nil();
List MakeListFromArray(unsigned int count, Term * terms[]);
List MakeList(unsigned int count, ...);
Term * Redex(List redex);
Term * ConstantStringFromConstantStr(ConstantStr str);
Term * ConstantStringFromConstantLimitedStr(ConstantLimitedStr str);
Term * ConstantString(LimitedStr str);
Term * Symbol(LimitedStr str);
Term * SymbolFromConstantStr(ConstantStr str);
Term * SymbolFromConstantLimitedStr(ConstantLimitedStr str);
Term * LazyFunction(LazyFunctionPtr lazyFunction);
Term * DefineFunction(List formalArguments, List body, ContextBindings * context);
Term * Empty();
Term * Character(Chr character);
Term * FileDescriptor(int fildes);