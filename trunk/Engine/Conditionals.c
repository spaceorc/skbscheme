#include "Conditionals.h"
#include "Memory.h"

Term * True() {
	Term * result = AllocateTerm(terBoolean);
	result->boolean = 1;
	return result;
}

Term * False() {
	Term * result = AllocateTerm(terBoolean);
	result->boolean = 0;
	return result;
}

int IsTrue(Term * term) {
	return !IsFalse(term);
}

int IsFalse(Term * term) {
	return term->tag == terBoolean && !term->boolean;
}