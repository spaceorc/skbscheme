#include "Variable.h"
#include "MemoryManager.h"

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
