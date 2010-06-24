#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

#include "FileDescriptor.h"
#include "Constructors.h"
#include "Error.h"

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
		RaisePosixError(errno);
	str.size = size;
	return ConstantString(str);
}

Term * FunctionWrite(List arguments) {
	Term * args[] = {0, 0}, * error = 0;
	int size;
	if (TakeSeveralArguments(arguments, args, &error) < 0)
		return error;
	if (terFileDescriptor != args[0]->tag)
		return InvalidArgumentType();
	if (terConstantString != args[1]->tag)
		return InvalidArgumentType();
	size = write(args[0]->fildes, args[1]->constantString.str, args[1]->constantString.size);
	if (size < 0)
		RaisePosixError(errno);
	return Number(size);
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

#pragma warning(default:4996)