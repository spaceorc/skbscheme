#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "Parser.h"
#include "Number.h"
#include "Pair.h"

char IterateSymbol(ConstLimitedStr * symbol) {
	char result = 0;
	if (symbol->size > 0) {
		result = *(symbol->str);
		symbol->str++;
		symbol->size--;
	}
	return result;
}

Term * parseNumber(ConstLimitedStr symbol) {
	int n = 0;
	char c;
	while (c = IterateSymbol(&symbol))
		n = n * 10 + (c - '0');
	return Number(n);
}

int isNumber(ConstLimitedStr symbol) {
	char c;
	while (c = IterateSymbol(&symbol)) {
		if (c < '0')
			return 0;
		if (c > '9')
			return 0;
	}
	return 1;
}

FunctionPtr FindBoundFunction(ConstLimitedStr symbol, Context * context) {
	List contextBindings;
	Term * keyValuePair;
	while (0 != context) {
		contextBindings = context->bindings;
		context = context->previous;
		while (keyValuePair = IterateList(&contextBindings)) {
			assert(tagPair == keyValuePair->tag);
			assert(tagConstantString == keyValuePair->pair->first->tag);
			assert(tagFunction == keyValuePair->pair->second->tag);
			if (0 == StrCompare(keyValuePair->pair->first->constStr, symbol))
				return keyValuePair->pair->second->function;
		}
	}
	return 0;
}

Term * ParseTerm(ConstLimitedStr symbol, Context * context) {
	FunctionPtr functionPtr;
	if (isNumber(symbol))
		return parseNumber(symbol);
	functionPtr = FindBoundFunction(symbol, context);
	if (0 == functionPtr)
		return InvalidSymbol();
	return Function(functionPtr);
}

Context * AllocateContext(Context * previous) {
	Context * result = malloc(sizeof(Context));
	result->previous = previous;
	result->redex = 0;
	result->bindings = 0;
	return result;
}

Term * InvalidClosingBracket() {
	Term * result = AllocateTerm(tagError);
	result->message = "Invalid closing bracket";
	return result;
}

Term * Parse(Token token, Context ** context) {
	Term * term = 0;
	switch(token.tag) {
		case tokSymbol:
			assert(token.range.size > 0);
			term = ParseTerm(token.range, *context);
			if (term->tag == tagError)
				break;
			assert(term->tag != tagRedex);
			if (0 == (*context)->previous)
				break;
			(*context)->redex = AppendListElement((*context)->redex, term);
			term = 0;
			break;
		case tokOpeningBracket:
			*context = AllocateContext(*context);
			break;
		case tokClosingBracket:
			if (0 == (*context)->previous)
				return InvalidClosingBracket();
			term = AllocateTerm(tagRedex);
			term->redex = (*context)->redex;
			*context = (*context)->previous;
			break;
		default:
			assert(0);
	}
	return term;
}

const char * globalNames [] = {"+", "-"};
FunctionPtr globalPointers [] = {OperatorPlus, OperatorMinus};

void AddBindingToContext(Context * context, ConstLimitedStr name, Term * value) {
	context->bindings = AppendListElement(context->bindings, InternalCons(ConstantStringFromLimited(name), value));
}

Context * AcquireContext() {
	Context * result = AllocateContext(0);
	int len = sizeof(globalNames)/sizeof(globalNames[0]);
	assert(len == (sizeof(globalPointers)/sizeof(globalPointers[0])));
	while (len-- > 0)
		result->bindings = AppendListElement(result->bindings, InternalCons(ConstantString(globalNames[len]), Function(globalPointers[len])));
	return result;
}

int CanFinishParsing(Context * context) {
	return 0 == context->previous;
}