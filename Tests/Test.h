#pragma once

#include <stdio.h>

class AssertException {
private:
	char m_message[4096];
public:
	AssertException(const char * message, const char * function, const char * file, int line) {
		sprintf(m_message, "[%s] in [%s] at [%s] line [%d]", message, function, file, line);
	}
	const char * message() const {
		return m_message;
	}
};

void RunTestMethod(const char * methodName, void (*methodPointer) ()) {
	try {
		printf("[%s]\t\tSTARTED\n", methodName);
		methodPointer();
		printf("[%s]\t\tOK\n", methodName);
	}
	catch (const AssertException & e) {
		printf("[%s]\t\tFAILED\t\t%s\n", methodName, e.message());
	}
	catch (...) {
		printf("[%s]\t\tEXCEPTED\n", methodName);
	}
}

void AssertCondition(const char * message, bool condition, const char * function, const char * file, int line) {
	if (!condition)
		throw AssertException(message, function, file, line);
}

#define RunTest(test) RunTestMethod(#test, test)
#define AssertThat(condition) AssertCondition(#condition, (condition), __FUNCTION__, __FILE__, __LINE__)