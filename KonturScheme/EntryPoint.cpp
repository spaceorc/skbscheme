#include <iostream>

extern "C" {
	#include "Tokenizer.h"
	#include "Parser.h"
	#include "Redex.h"
	#include "FileDescriptor.h"
}

int main() {
	ParserContext * context = AcquireParserContext();
	ContextBindings * contextBindings = AcquireContextBindings();
	while (1) {
		if (CanFinishParsing(context))
			std::cout << "> ";
		Term * str = FunctionRead(MakeList(2, StdIn(), Number(1024)));
		if (terConstantString != str->tag)
			return 0;
		Token token;
		while (tokEnd != (token = GetToken(&(str->constantString))).tag) {
			Term * term;
			if (term = Parse(token, &context)) {
				if (terEmpty != FunctionWriteTerm(MakeList(2, StdOut(), Eval(term, contextBindings)))->tag) {
					std::cerr << "Failed to write term to console" << std::endl;
					return 1;
				}
			}
		}
	}
	return 0;
}