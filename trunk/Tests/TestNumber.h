#include "Test.h"

TEST(NumberPlus) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(3), Eval(ParseSingle("(+ 1 2)"), contextBindings));
}

TEST(NumberPlusNoArguments) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(+)"), contextBindings));
}

TEST(NumberPlus3Arguments) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(6), Eval(ParseSingle("(+ 1 2 3)"), contextBindings));
}

TEST(NumberPlus1Argument) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(+ 1)"), contextBindings));
}

TEST(NumberPlus1stIsNotANumber) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(+ #\\c 1)"), contextBindings));
}

TEST(NumberPlus3Arguments2ndIsNotANumber) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(+ 1 #\\c 2)"), contextBindings));
}

TEST(NumberMinus) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(3), Eval(ParseSingle("(- 5 2)"), contextBindings));
}

TEST(NumberMinusNoArguments) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(-)"), contextBindings));
}

TEST(NumberMinus1Argument) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(-1), Eval(ParseSingle("(- 1)"), contextBindings));
}

TEST(NumberMinus3Arguments) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(-4), Eval(ParseSingle("(- 1 2 3)"), contextBindings));
}

TEST(NumberMinus1stIsNotANumber) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(- #\\c 1)"), contextBindings));
}

TEST(NumberEq) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(True(), Eval(ParseSingle("(= 1 1 1)"), contextBindings));
	AssertEq(False(), Eval(ParseSingle("(= 1 1 2)"), contextBindings));
}

TEST(NumberEqWith1Argument) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(= 1)"), contextBindings));
}

TEST(NumberEqWithNotANumber) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(= 1 2 #\\c)"), contextBindings));
}

TEST(IsNumber) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(True(), Eval(ParseSingle("(number? 1)"), contextBindings));
	AssertEq(False(), Eval(ParseSingle("(number? #t)"), contextBindings));
}