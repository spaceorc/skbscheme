#include "Test.h"

TEST(IsString) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(True(), Eval(ParseSingle("(string? \"lalala\")"), contextBindings));
	AssertEq(False(), Eval(ParseSingle("(string? #t)"), contextBindings));
}

TEST(MakeString) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(make-string 5)"), contextBindings));
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(make-string #t #\\c)"), contextBindings));
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(make-string 5 #t)"), contextBindings));
	AssertEq(StringFromConstantStr(STR("ccccc")), Eval(ParseSingle("(make-string 5 #\\c)"), contextBindings));
}

TEST(StringLength) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(string-length)"), contextBindings));
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(string-length 5)"), contextBindings));
	AssertEq(Number(6), Eval(ParseSingle("(string-length \"lalala\")"), contextBindings));
}

TEST(StringRef) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(string-ref)"), contextBindings));
	AssertEq(Character('c'), Eval(ParseSingle("(string-ref \"abcdef\" 2)"), contextBindings));
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(string-ref 2 \"abcdef\")"), contextBindings));
	AssertEq(ContractError(), Eval(ParseSingle("(string-ref \"abcdef\" (- 1))"), contextBindings));
	AssertEq(ContractError(), Eval(ParseSingle("(string-ref \"abcdef\" 6)"), contextBindings));
}

TEST(StringSet) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Empty(), Eval(ParseSingle("(define (lalala str k char) (string-set! str k char) str)"), contextBindings));
	AssertEq(StringFromConstantStr(STR("abZdef")), Eval(ParseSingle("(lalala \"abcdef\" 2 #\\Z)"), contextBindings));
	AssertEq(Empty(), Eval(ParseSingle("(string-set! \"abcdef\" 2 #\\Z)"), contextBindings));
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(string-set! \"abcdef\" 2)"), contextBindings));
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(string-set! 2 \"abcdef\" #\\Z)"), contextBindings));
	AssertEq(ContractError(), Eval(ParseSingle("(string-set! \"abcdef\" (- 1) \\Z)"), contextBindings));
	AssertEq(ContractError(), Eval(ParseSingle("(string-set! \"abcdef\" 6 \\Z)"), contextBindings));
}

TEST(Substring) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(substring)"), contextBindings));
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(substring 1)"), contextBindings));
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(substring 2 \"abcdef\")"), contextBindings));
	AssertEq(ContractError(), Eval(ParseSingle("(substring \"abcdef\" (- 1))"), contextBindings));
	AssertEq(ContractError(), Eval(ParseSingle("(substring \"abcdef\" 6)"), contextBindings));
	AssertEq(ContractError(), Eval(ParseSingle("(substring \"abcdef\" 2 (- 1))"), contextBindings));
	AssertEq(ContractError(), Eval(ParseSingle("(substring \"abcdef\" 2 1)"), contextBindings));
	AssertEq(ContractError(), Eval(ParseSingle("(substring \"abcdef\" 2 6)"), contextBindings));
	AssertEq(StringFromConstantStr(STR("cdef")), Eval(ParseSingle("(substring \"abcdef\" 2)"), contextBindings));
	AssertEq(StringFromConstantStr(STR("cde")), Eval(ParseSingle("(substring \"abcdef\" 2 4)"), contextBindings));
}