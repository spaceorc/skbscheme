#pragma once

#include "Tokenizer.h"
#include "Number.h"
#include "Redex.h"

struct structContext;
typedef struct structContext Context;

struct structContext {
	List redex;
	Context * previous;
};

Term * Parse(Token token, Context ** context) {
	return Nil();
}