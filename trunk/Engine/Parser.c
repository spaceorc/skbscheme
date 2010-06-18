#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "Dictionary.h"
#include "Parser.h"
#include "Number.h"
#include "Pair.h"

Term * InvalidSymbol() {
	Term * result = AllocateTerm(terError);
	result->message = "Invalid symbol";
	return result;
}

Term * FindBoundTerm(ConstLimitedStr symbol, Context * context) {
	Term * result = InvalidSymbol();
	while (0 != context) {
		result = InternalFind(context->bindings, symbol);
		if (result != 0)
			return result;
		context = context->previous;
	}
	return InvalidSymbol();
}

Term * ParseTerm(ConstLimitedStr symbol, Context * context) {
	if (isNumber(symbol))
		return parseNumber(symbol);
	return FindBoundTerm(symbol, context);
}

Context * AllocateContext(Context * previous) { // todo one more place to handle with memory
	Context * result = malloc(sizeof(Context));
	result->previous = previous;
	result->redex = 0;
	result->bindings = 0;
	return result;
}

Term * InvalidClosingBracket() {
	Term * result = AllocateTerm(terError);
	result->message = "Invalid closing bracket";
	return result;
}

Term * Parse(Token token, Context ** context) {
	Term * term = 0;
	switch(token.tag) {
		case tokSymbol:
			assert(token.range.size > 0);
			term = ParseTerm(token.range, *context);
			if (term->tag == terError)
				break;
			assert(term->tag != terRedex);
			if (0 == (*context)->previous)
				break;
			(*context)->redex = InternalAppend((*context)->redex, term);
			term = 0;
			break;
		case tokOpeningBracket:
			*context = AllocateContext(*context);
			break;
		case tokClosingBracket:
			if (0 == (*context)->previous)
				return InvalidClosingBracket();
			term = AllocateTerm(terRedex);
			term->redex = (*context)->redex;
			*context = (*context)->previous;
			break;
		default:
			assert(0);
	}
	return term;
}

const char * globalNames [] = {"+", "-", "cons", "car", "cdr"};
FunctionPtr globalPointers [] = {OperatorPlus, OperatorMinus, FunctionCons, FunctionCar, FunctionCdr};

void AddBindingToContext(Context * context, ConstLimitedStr name, Term * value) {
	context->bindings = InternalSetFromLimited(context->bindings, name, value);
}

Context * AcquireContext() {
	Context * result = AllocateContext(0);
	int len = sizeof(globalNames)/sizeof(globalNames[0]);
	assert(len == (sizeof(globalPointers)/sizeof(globalPointers[0])));
	while (len-- > 0)
		result->bindings = InternalSet(result->bindings, globalNames[len], Function(globalPointers[len]));
	return result;
}

int CanFinishParsing(Context * context) {
	return 0 == context->previous;
}