#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

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
			current->second = AllocateTerm(terPair);
			current->second->pair = next;
		}
		current = next;
		if (result == 0)
			result = current;
	}
	return result;
}

Term * MakeRedex(int argc, ...) {
	va_list listPointer;
	va_start(listPointer, argc);
	List result = 0;
	Pair * current = 0;
	for(int i = 0; i < argc; i++) {
		Pair * next = AllocatePair();
		next->first = va_arg(listPointer, Term *);
		next->second = Nil();
		if (current != 0) {
			current->second = AllocateTerm(terPair);
			current->second->pair = next;
		}
		current = next;
		if (result == 0)
			result = current;
	}
	Term * term = AllocateTerm(terRedex);
	term->redex = result;
	return term;
}

Term * ParseSingle(ConstantStr expression) {
	Token token;
	Term * result = 0;
	LimitedStr limitedExpression = LimitedStrFromConstantStr(expression);
	ParserContext * context = AcquireParserContext();
	while(tokEnd != (token = GetToken(&limitedExpression)).tag) {
		result = Parse(token, &context);
	}
	return result;
}

const char * DumpTag(int tag) {
	switch(tag) {
		case terNumber:
			return "Number";
		case terFunction:
			return "Function";
		case terPair:
			return "Pair";
		case terError:
			return "Error";
		case terNil:
			return "Nil";
		case terRedex:
			return "Redex";
		case terConstantString:
			return "Constant string";
		case terSymbol:
			return "Symbol";
		case terLazyFunction:
			return "Lazy function";
		case terDefinedFunction:
			return "Defined function";
		default:
			assert(0);
			return 0;
	}
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

const char * DumpTok(int tag) {
	switch(tag) {
		case tokOpeningBracket:
			return "OpeningBracket";
		case tokClosingBracket:
			return "ClosingBracket";
		case tokSymbol:
			return "Symbol";
		case tokEscape:
			return "Escape";
		case tokQuotedString:
			return "Quoted string";
		case tokEnd:
			return "End";
		case tokError:
			return "Error";
		default:
			assert(0);
			return 0;
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
	switch(expected->tag) {
		case terNumber:
			if (expected->number != was->number) {
				sprintf_s(message, "expected number %d, but was number %d", expected->number, was->number);
				AssertCondition(message, false, function, file, line);
			}
			break;
		case terRedex:
		case terPair:
			AssertEqCondition(expected->pair->first, was->pair->first, function, file, line);
			AssertEqCondition(expected->pair->second, was->pair->second, function, file, line);
			break;
		case terSymbol:
		case terConstantString:
			if (Compare(expected->constantString, was->constantString)) {
				sprintf_s(message, "expected const string '%.*s', but was const string '%.*s'", expected->constantString.size, expected->constantString.str, was->constantString.size, was->constantString.str);
				AssertCondition(message, false, function, file, line);
			}
		case terNil:
		case terError:
			break;
		case terFunction:
		case terLazyFunction:
			if (expected->function != was->function) {
				sprintf_s(message, "expected function pointer %p, but was function pointer %p", expected->function, was->function);
				AssertCondition(message, false, function, file, line);
			}
			break;
		default:
			assert(0);
	}
}

#define AssertSymbol(expected, was) AssertSymbolCondition(expected, was, __FUNCTION__, __FILE__, __LINE__)
#define AssertQuotedString(expected, was) AssertQuotedStringCondition(expected, was, __FUNCTION__, __FILE__, __LINE__)
#define AssertBracket(expectedTag, expectedRange, was) AssertBracketCondition(expectedTag, expectedRange, was, __FUNCTION__, __FILE__, __LINE__)

void AssertSymbolCondition(ConstantStr expected, Token was, const char * function, const char * file, int line) {
	char message[1024];
	AssertTokCondition(tokSymbol, was, function, file, line);
	if (0 != CompareConstantLimitedStr(LimitConstantStr(expected), ConstLimitedStr(was.range))) {
		sprintf_s(message, "expected symbol '%s', but was symbol '%.*s'", expected, was.range.size, was.range.str);
		AssertCondition(message, false, function, file, line);
	}
}

void AssertQuotedStringCondition(ConstantStr expected, Token was, const char * function, const char * file, int line) {
	char message[1024];
	AssertTokCondition(tokQuotedString, was, function, file, line);
	if (0 != CompareConstantLimitedStr(LimitConstantStr(expected), ConstLimitedStr(was.range))) {
		sprintf_s(message, "expected quoted string '%s', but was quoted string '%.*s'", expected, was.range.size, was.range.str);
		AssertCondition(message, false, function, file, line);
	}
}

void AssertBracketCondition(int expectedTag, ConstantStr expectedRange, Token was, const char * function, const char * file, int line) {
	char message[1024];
	AssertTokCondition(expectedTag, was, function, file, line);
	if (0 != CompareConstantLimitedStr(LimitConstantStr(expectedRange), ConstLimitedStr(was.range))) {
		sprintf_s(message, "expected bracket '%s', but was bracket '%.*s'", expectedRange, was.range.size, was.range.str);
		AssertCondition(message, false, function, file, line);
	}
}

#define STR(x) ((ConstantStr)(x))