#include "Redex.h"


Term * FunctionApply(List arguments) {
	Term * function = 0;
	function = IterateList(&arguments);
	return function->function(arguments);
}