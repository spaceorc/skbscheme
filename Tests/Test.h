#pragma once

#include <stdio.h>
#include <stdarg.h>

#include "Engine.h"

class AssertException {
private:
	char m_message[4096];
public:
	AssertException(const char * message, const char * function, const char * file, int line) {
		sprintf_s(m_message, "[%s] in [%s] at [%s] line [%d]", message, function, file, line);
	}
	const char * message() const {
		return m_message;
	}
};

void RunTestMethod(const char * methodName, void (*methodPointer) ()) {
}

void AssertCondition(const char * message, bool condition, const char * function, const char * file, int line) {
	if (!condition)
		throw AssertException(message, function, file, line);
}

#define AssertThat(condition) AssertCondition(#condition, (condition), __FUNCTION__, __FILE__, __LINE__)

class TestFixture {
private:
	typedef void (*TestMethod)();
	static int testCount;
	static int succeededTestCount;
	static int failedTestCount;
	static int exceptedTestCount;
	static TestFixture tests[1024];
	TestMethod m_testMethod;
	const char * m_methodName;
public:
	static void RunTests();
	TestFixture(TestMethod testMethod, const char * methodName);
	TestFixture();
	void Run();
};

TestFixture::TestFixture() : m_testMethod(0), m_methodName(0) {
}

TestFixture::TestFixture(TestMethod testMethod, const char * methodName) : m_testMethod(testMethod), m_methodName(methodName) {
	tests[testCount++] = *this;
}

void TestFixture::Run() {
	try {
		printf("[%s]\t\tstarted", m_methodName);
		m_testMethod();
		printf("\tOK\n");
		succeededTestCount++;
	}
	catch (const AssertException & e) {
		printf("\tFAILED\t\t%s\n", e.message());
		failedTestCount++;
	}
	catch (...) {
		printf("\tEXCEPTED\n");
		exceptedTestCount++;
	}
}

int TestFixture::succeededTestCount = 0;
int TestFixture::failedTestCount = 0;
int TestFixture::exceptedTestCount = 0;
int TestFixture::testCount = 0;
TestFixture TestFixture::tests[1024];

void TestFixture::RunTests() {
	for(int i = 0; i < testCount; ++i)
		tests[i].Run();
	printf("\ntotal: %d; succeeded: %d; failed: %d; excepted: %d\n", testCount, succeededTestCount, failedTestCount, exceptedTestCount);
	if (exceptedTestCount > 0)
		printf("!!! TESTS EXCEPTED !!!\n");
	else if (failedTestCount > 0)
		printf("!!! TESTS FAILED !!!\n");
}

#define CONCAT2(x,y) x##y
#define CONCAT(x,y) CONCAT2(x,y)
#define TEST(x) void CONCAT(Test,x)(); TestFixture CONCAT(testFixture,__COUNTER__)(CONCAT(Test,x), #x); void CONCAT(Test,x)()

List MakeList(int argc, ...) {
	va_list listPointer;
	va_start(listPointer, argc);
	List result = 0;
	Pair * current = 0;
	for(int i = 0; i < argc; i++) {
		Pair * next = AllocatePair();
		next->first = va_arg(listPointer, Term *);
		next->second = Nil();
		if (current != 0) {
			current->second = AllocateTerm(tagPair);
			current->second->pair = next;
		}
		current = next;
		if (result == 0)
			result = current;
	}
	return result;
}

#define AssertTag(expected, term) AssertTagCondition(expected, term, __FUNCTION__, __FILE__, __LINE__)
#define AssertTok(expected, token) AssertTokCondition(expected, token, __FUNCTION__, __FILE__, __LINE__)

void AssertTagCondition(int expected, Term * term, const char * function, const char * file, int line) {
	char message[1024];
	if (term == 0) {
		sprintf_s(message, "expected term with tag %s, but was null pointer", DumpTag(expected));
		AssertCondition(message, false, function, file, line);
	}
	if (expected != term->tag) {
		sprintf_s(message, "expected term with tag %s, but was term with tag %s", DumpTag(expected), DumpTag(term->tag));
		AssertCondition(message, false, function, file, line);
	}
}

void AssertTokCondition(int expected, Token token, const char * function, const char * file, int line) {
	char message[1024];
	if (expected != token.tag) {
		sprintf_s(message, "expected token with tag %s, but was token with tag %s", DumpTok(expected), DumpTok(token.tag));
		AssertCondition(message, false, function, file, line);
	}
}

#define AssertEq(expected, was) AssertEqCondition(expected, was, __FUNCTION__, __FILE__, __LINE__)

void AssertEqCondition(Term * expected, Term * was, const char * function, const char * file, int line) {
	char message[1024];
	AssertTagCondition(expected->tag, was, function, file, line);
	if (expected->number != was->number) {
		sprintf_s(message, "expected number %d, but was number %d", expected->number, was->number);
		AssertCondition(message, false, function, file, line);
	}
}
