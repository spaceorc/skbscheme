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
	AssertEq(ConstantStringFromConstantStr(STR("ccccc")), Eval(ParseSingle("(make-string 5 #\\c)"), contextBindings));
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