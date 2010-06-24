#include <fcntl.h>
#include <assert.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

#include "FileDescriptor.h"
#include "Constructors.h"
#include "Error.h"

// todo functional tests required

// warning C4996: 'open': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _open. See online help for details.
// warning C4996: 'close': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _open. See online help for details.
// warning C4996: 'read': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _open. See online help for details.
// warning C4996: 'write': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _open. See online help for details.
#pragma warning(disable:4996)

Term * FunctionOpen(List arguments) {
	Term * args[] = {0, 0, 0}, * error = 0;
	LimitedStr filename;
	int fd;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (terConstantString != args[0]->tag)
		return InvalidArgumentType();
	if (terNumber != args[1]->tag)
		return InvalidArgumentType();
	if (terNumber != args[2]->tag)
		return InvalidArgumentType();
	filename = DeepCopy(args[0]->constantString);
	fd = open(filename.str, args[1]->number, args[2]->number);
	if (fd < 0)
		return RaisePosixError(errno);
	return FileDescriptor(fd);
}

Term * FunctionClose(List arguments) {
	Term * arg = 0, *error = 0;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (terFileDescriptor != arg->tag)
		return InvalidArgumentType();
	if (close(arg->fildes))
		return RaisePosixError(errno);
	return Empty();
}

Term * FunctionRead(List arguments) {
	Term * args[] = {0, 0}, * error = 0;
	int size;
	LimitedStr str;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (terFileDescriptor != args[0]->tag)
		return InvalidArgumentType();
	if (terNumber != args[1]->tag)
		return InvalidArgumentType();
	str = AllocateLimitedStr(args[1]->number);
	size = read(args[0]->fildes, str.str, str.size);
	if (size < 0)
		return RaisePosixError(errno);
	str.size = size;
	return ConstantString(str);
}

Term * InternalWrite(int fildes, LimitedStr str) {
	LimitedStr zeroStr = DeepCopy(str);
	int size = write(fildes, zeroStr.str, zeroStr.size);
	if (size < 0)
		return RaisePosixError(errno);
	return Number(size);
}

#pragma warning(default:4996)

Term * InternalWriteConstantStr(int fildes, ConstantStr str) {
	return InternalWrite(fildes, LimitedStrFromConstantStr(str));
}

Term * InternalWritePair(int fildes, Pair * term);

Term * InternalWriteTerm(int fildes, Term * term) {
	Chr buffer[1024] = "";
	Term * result = 0;
	switch(term->tag) {
		case terNumber:
			sprintf_s(buffer, 1024, "%d\n", term->number);
			break;
		case terFileDescriptor:
			sprintf_s(buffer, 1024, "fildes %d\n", term->fildes);
			break;
		case terCharacter:
			sprintf_s(buffer, 1024, "#\\%c\n", (int)term->character);
			break;
		case terSymbol:
			sprintf_s(buffer, 1024, "%.*s\n", term->symbol.size, term->symbol.str);
			break;
		case terConstantString:
			sprintf_s(buffer, 1024, "\"%.*s\"\n", term->constantString.size, term->constantString.str);
			break;
		case terEmpty:
			return Empty();
		case terNil:
			sprintf_s(buffer, 1024, "()\n");
			break;
		case terError:
			sprintf_s(buffer, 1024, "error \"%.*s\"\n", term->message.size, term->message.str);
			break;
		case terFunction:
			sprintf_s(buffer, 1024, "built-in function\n");
			break;
		case terLazyFunction:
			sprintf_s(buffer, 1024, "lazy function\n");
			break;
		case terDefinedFunction:
			sprintf_s(buffer, 1024, "function\n");
			break;
		case terBoolean:
			if (term->boolean)
				sprintf_s(buffer, 1024, "#t\n");
			else
				sprintf_s(buffer, 1024, "#f\n");
			break;
		case terPair:
			return InternalWritePair(fildes, term->pair);
		default:
			assert(0);
	}
	term = InternalWriteConstantStr(fildes, buffer);
	if (terError == term->tag)
		return term;
	return Empty();
}

Term * FunctionWrite(List arguments) {
	Term * args[] = {0, 0}, * error = 0;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (terFileDescriptor != args[0]->tag)
		return InvalidArgumentType();
	if (terConstantString != args[1]->tag)
		return InvalidArgumentType();
	return InternalWrite(args[0]->fildes, args[1]->constantString);
}

Term * FunctionWriteTerm(List arguments) {
	Term * args[] = {0, 0}, * error = 0;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (terFileDescriptor != args[0]->tag)
		return InvalidArgumentType();
	return InternalWriteTerm(args[0]->fildes, args[1]);
}

Term * StdIn() {
	return FileDescriptor(0);
}

Term * StdOut() {
	return FileDescriptor(1);
}

Term * StdErr() {
	return FileDescriptor(2);
}

Term * InternalWritePair(int fildes, Pair * pair) {
	Term * term = InternalWriteConstantStr(fildes, "(");
	if (terError == term->tag)
		return term;
	while(pair) {
		term = InternalWriteTerm(fildes, pair->first);
		if (terError == term->tag)
			return term;
		switch(pair->second->tag) {
			case terPair:
				pair = pair->second->pair;
				break;
			case terNil:
				pair = 0;
				break;
			default:
				term = InternalWriteConstantStr(fildes, " . ");
				if (terError == term->tag)
					return term;
				InternalWriteTerm(fildes, pair->second);
				pair = 0;
				break;
		}
	}
	term = InternalWriteConstantStr(fildes, ")\n");
	if (terError == term->tag)
		return term;
	return Empty();
}