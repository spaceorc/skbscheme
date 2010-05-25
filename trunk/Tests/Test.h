#pragma once

#include <stdio.h>

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
	typedef void (*TestMethod)();

private:
	TestMethod m_testMethod;
	const char * m_methodName;
public:
	static void RunTests();
	TestFixture(TestMethod testMethod, const char * methodName);
	void Run();
};

int testCount = 0, succeededTestCount, failedTestCount, exceptedTestCount;
TestFixture* tests[1024];
TestFixture::TestFixture(TestMethod testMethod, const char * methodName) : m_testMethod(testMethod), m_methodName(methodName) {
	tests[testCount++] = this;
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

void TestFixture::RunTests() {
	succeededTestCount = 0;
	failedTestCount = 0;
	exceptedTestCount = 0;
	for(int i = 0; i < testCount; ++i) {
		tests[i]->Run();
	}
	printf("\ntotal: %d; succeeded: %d; failed: %d; excepted: %d\n", testCount, succeededTestCount, failedTestCount, exceptedTestCount);
	if (exceptedTestCount > 0)
		printf("!!! TESTS EXCEPTED !!!\n");
	else if (failedTestCount > 0)
		printf("!!! TESTS FAILED !!!\n");
}

#define CONCAT2(x,y) x##y
#define CONCAT(x,y) CONCAT2(x,y)
#define TEST(x) void CONCAT(Test,x)(); TestFixture CONCAT(testFixture,__LINE__)(CONCAT(Test,x), #x); void CONCAT(Test,x)()