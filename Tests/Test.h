#pragma once

#include <stdio.h>
#include <assert.h>

#include "Engine.h"

#define STR(x) ((ConstantStr)(x))

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

Term * ParseSingle(const char * expression) {
	Token token;
	Term * result = 0;
	LimitedStr limitedExpression = LimitedStrFromConstantStr(STR(expression));
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
		case terString:
			return "String";
		case terSymbol:
			return "Symbol";
		case terLazyFunction:
			return "Lazy function";
		case terLambda:
			return "Lambda";
		case terEmpty:
			return "Empty";
		case terBoolean:
			return "Boolean";
		case terCharacter:
			return "Character";
		case terFileDescriptor:
			return "File descriptor";
		default:
			assert(0);
			return 0;
	}
}

#define AssertTag(expected, term) AssertTagCondition(expected, term, __FUNCTION__, __FILE__, __LINE__)
#define AssertTok(expected, token) AssertTokCondition(expected, token, __FUNCTION__, __FILE__, __LINE__)

void AssertTagCondition(int expectedTag, Term * was, const char * function, const char * file, int line) {
	const unsigned int size = 1024;
	char message[size];
	unsigned int offset = 0;
	if (was == 0) {
		sprintf_s(message, "expected term with tag %s, but was null pointer", DumpTag(expectedTag));
		AssertCondition(message, false, function, file, line);
	}
	if (expectedTag != was->tag) {
		offset = sprintf_s(message, "expected term with tag %s, but was ", DumpTag(expectedTag));
		DumpTerm((Chr *)(message + offset), size - offset, was);
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
		case terFileDescriptor:
			if (expected->fildes != was->fildes) {
				sprintf_s(message, "expected file descriptor %d, but was file descriptor %d", expected->number, was->number);
				AssertCondition(message, false, function, file, line);
			}
			break;
		case terRedex:
		case terPair:
			AssertEqCondition(expected->pair->first, was->pair->first, function, file, line);
			AssertEqCondition(expected->pair->second, was->pair->second, function, file, line);
			break;
		case terSymbol:
			if (Compare(expected->symbol, was->symbol)) {
				sprintf_s(message, "expected const string '%.*s', but was const string '%.*s'", expected->symbol.size, expected->symbol.str, was->symbol.size, was->symbol.str);
				AssertCondition(message, false, function, file, line);
			}
			break;
		case terString:
			if (Compare(expected->string, was->string)) {
				sprintf_s(message, "expected const string '%.*s', but was const string '%.*s'", expected->string.size, expected->string.str, was->string.size, was->string.str);
				AssertCondition(message, false, function, file, line);
			}
			break;
		case terCharacter:
			if (expected->character != was->character) {
				sprintf_s(message, "expected character '%c', but was character '%c'", expected->character, was->character);
				AssertCondition(message, false, function, file, line);
			}
			break;
		case terNil:
		case terEmpty:
		case terError:
			break;
		case terFunction:
		case terLazyFunction:
			if (expected->function != was->function) {
				sprintf_s(message, "expected function pointer %p, but was function pointer %p", expected->function, was->function);
				AssertCondition(message, false, function, file, line);
			}
			break;
		case terBoolean:
			if (expected->boolean != was->boolean) {
				sprintf_s(message, "expected boolean %s, but was boolean %s", expected->boolean ? "True" : "False", was->boolean ? "True" : "False");
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

void AssertSymbolCondition(ConstantStr expectedText, Token was, const char * function, const char * file, int line) {
	char message[1024];
	AssertTokCondition(tokSymbol, was, function, file, line);
	if (0 != CompareConstantLimitedStr(LimitConstantStr(expectedText), ConstLimitedStr(was.text))) {
		sprintf_s(message, "expected symbol '%s', but was symbol '%.*s'", expectedText, was.text.size, was.text.str);
		AssertCondition(message, false, function, file, line);
	}
}

void AssertQuotedStringCondition(ConstantStr expectedText, Token was, const char * function, const char * file, int line) {
	char message[1024];
	AssertTokCondition(tokQuotedString, was, function, file, line);
	if (0 != CompareConstantLimitedStr(LimitConstantStr(expectedText), ConstLimitedStr(was.text))) {
		sprintf_s(message, "expected quoted string '%s', but was quoted string '%.*s'", expectedText, was.text.size, was.text.str);
		AssertCondition(message, false, function, file, line);
	}
}

void AssertBracketCondition(int expectedTag, ConstantStr expectedText, Token was, const char * function, const char * file, int line) {
	char message[1024];
	AssertTokCondition(expectedTag, was, function, file, line);
	if (0 != CompareConstantLimitedStr(LimitConstantStr(expectedText), ConstLimitedStr(was.text))) {
		sprintf_s(message, "expected bracket '%s', but was bracket '%.*s'", expectedText, was.text.size, was.text.str);
		AssertCondition(message, false, function, file, line);
	}
}