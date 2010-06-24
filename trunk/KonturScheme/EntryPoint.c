#include <stdio.h>

#include "Tokenizer.h"
#include "Parser.h"
#include "Redex.h"
#include "FileDescriptor.h"
#include "Library.h"

int main() {
	ParserContext * context = AcquireParserContext();
	ContextBindings * contextBindings = AcquireContextBindings();
	while (1) {
		Term * str = 0;
		Token token;
		if (CanFinishParsing(context))
			printf("> ");
		str = FunctionRead(MakeList(2, StdIn(), Number(1024)));
		if (terConstantString != str->tag)
			return 0;
		while (tokEnd != (token = GetToken(&(str->constantString))).tag) {
			Term * term = Parse(token, &context);
			if (0 != term) {
				if (terEmpty != FunctionWriteTerm(MakeList(2, StdOut(), Eval(term, contextBindings)))->tag)
					return 0;
			}
		}
	}
	return 0;
}