#include "Term.h"

Term * FunctionOpen(List arguments);
Term * FunctionClose(List arguments);
Term * FunctionRead(List arguments);
Term * FunctionWrite(List arguments);
Term * FunctionWriteTerm(List arguments);
Term * StdIn();
Term * StdOut();
Term * StdErr();