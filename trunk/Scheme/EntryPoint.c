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
			printf("\n> ");
		str = FunctionRead(MakeList(2, StdIn(), Number(1024)));
		if (terString != str->tag)
			return 0;
		while (tokEnd != (token = GetToken(&(str->string))).tag) {
			Term * term = Parse(token, &context);
			if (0 != term) {
				Term * fileDescriptor = StdOut();
				if (terError == term->tag)
					fileDescriptor = StdErr();
				if (terVoid != FunctionWriteTerm(MakeList(2, fileDescriptor, Eval(term, contextBindings)))->tag)
					return 0;			
			}
		}
	}
	return 0;
}