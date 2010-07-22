#include <fcntl.h>
#include <assert.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

#include "FileDescriptor.h"
#include "List.h"

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
	if (terString != args[0]->tag)
		return InvalidArgumentType();
	if (terNumber != args[1]->tag)
		return InvalidArgumentType();
	if (terNumber != args[2]->tag)
		return InvalidArgumentType();
	filename = ZeroStr(args[0]->string);
	fd = open(filename.str, args[1]->number, args[2]->number);
	if (fd < 0)
		return PosixError(errno);
	return FileDescriptor(fd);
}

Term * FunctionClose(List arguments) {
	Term * arg = 0, *error = 0;
	if (TakeSingleArgument(arguments, &arg, &error) < 0)
		return error;
	if (terFileDescriptor != arg->tag)
		return InvalidArgumentType();
	if (close(arg->fildes))
		return PosixError(errno);
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
	str = CreateLimitedStr(args[1]->number);
	size = read(args[0]->fildes, str.str, str.size);
	if (size < 0)
		return PosixError(errno);
	str.size = size;
	return ConstantString(str);
}

Term * InternalWrite(int fildes, LimitedStr str) {
	LimitedStr zeroStr = ZeroStr(str);
	int size = write(fildes, zeroStr.str, zeroStr.size);
	if (size < 0)
		return PosixError(errno);
	return Number(size);
}

#pragma warning(default:4996)

Term * InternalWriteConstantStr(int fildes, ConstantStr str) {
	return InternalWrite(fildes, LimitedStrFromConstantStr(str));
}

unsigned int DumpPair(Chr * buffer, unsigned int size, Pair * pair) {
	unsigned int offset = sprintf_s(buffer, size, "(");
	offset += DumpTerm(buffer + offset, size - offset, pair->first);
	while (pair) {
		switch(pair->second->tag) {
			case terPair:
				pair = pair->second->pair;
				break;
			case terNil:
				pair = 0;
				break;
			default:
				offset += DumpTerm(buffer + offset, size - offset, pair->second);
				pair = 0;
				break;
		}	}
	return offset + sprintf_s(buffer + offset, size - offset, ")\n");
}

unsigned int DumpTerm(Chr * buffer, unsigned int size, Term * term) {
	switch(term->tag) {
		case terNumber:
			return sprintf_s(buffer, size, "%d\n", term->number);
		case terFileDescriptor:
			return sprintf_s(buffer, size, "fildes %d\n", term->fildes);
		case terCharacter:
			return sprintf_s(buffer, size, "#\\%c\n", (int)term->character);
		case terSymbol:
			return sprintf_s(buffer, size, "%.*s\n", term->symbol.size, term->symbol.str);
		case terString:
			return sprintf_s(buffer, size, "\"%.*s\"\n", term->string.size, term->string.str);
		case terEmpty:
			return 0;
		case terNil:
			return sprintf_s(buffer, size, "()\n");
		case terError:
			return sprintf_s(buffer, size, "error \"%.*s\"\n", term->message.size, term->message.str);
		case terFunction:
			return sprintf_s(buffer, size, "built-in function\n");
		case terLazyFunction:
			return sprintf_s(buffer, size, "lazy function\n");
		case terLambda:
			return sprintf_s(buffer, size, "function\n");
		case terBoolean:
			if (term->boolean)
				return sprintf_s(buffer, size, "#t\n");
			else
				return sprintf_s(buffer, size, "#f\n");
		case terPair:
			return DumpPair(buffer, size, term->pair);
		default:
			assert(0);
			return 0;
	}
}


Term * InternalWriteTerm(int fildes, Term * term) {
	Chr buffer[1024] = "";
	Term * result = 0;

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
	if (terString != args[1]->tag)
		return InvalidArgumentType();
	return InternalWrite(args[0]->fildes, args[1]->string);
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
