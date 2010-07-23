#pragma once

#include "Term.h"

Term * Number(int number);
Term * Function(FunctionPtr function);
Term * Cons(Term * first, Term * second);
Term * Error(LimitedStr str);
Term * Nil();
List MakeListFromArray(unsigned int count, Term * terms[]);
List MakeList(unsigned int count, ...);
Term * Redex(List redex);
Term * StringFromConstantStr(ConstantStr str);
Term * StringFromConstantLimitedStr(ConstantLimitedStr str);
Term * String(LimitedStr str);
Term * Variable(LimitedStr str);
Term * VariableFromConstantStr(ConstantStr str);
Term * VariableFromConstantLimitedStr(ConstantLimitedStr str);
Term * LazyFunction(LazyFunctionPtr lazyFunction, AcquireLazyEvaluationContextPtr acquireLazyEvaluationContext);
Term * MakeLambda(List formalArguments, List body, ContextBindings * context);
Term * Void();
Term * Character(Chr character);
Term * FileDescriptor(int fildes);