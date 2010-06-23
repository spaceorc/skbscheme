#include "Test.h"

TEST(True) {
	Term * z = True();
	AssertTag(terBoolean, z);
	AssertThat(1 == z->boolean);
	AssertThat(1 == IsTrue(z));
	AssertThat(0 == IsFalse(z));
}

TEST(False) {
	Term * z = False();
	AssertTag(terBoolean, z);
	AssertThat(0 == z->boolean);
	AssertThat(0 == IsTrue(z));
	AssertThat(1 == IsFalse(z));
}

TEST(AnythingExceptFalseIsTrue) {
	AssertThat(1 == IsTrue(Number(1)));
	AssertThat(0 == IsFalse(Number(1)));
	AssertThat(1 == IsTrue(Number(0)));
	AssertThat(0 == IsFalse(Number(0)));
}

TEST(EvalOfOperatorNumberEq) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(False(), Eval(ParseSingle("(= 1 2)"), contextBindings));
}

TEST(And) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(1), Eval(ParseSingle("(and #t 1)"), contextBindings));
	AssertEq(Number(1), Eval(ParseSingle("(and #t (+ 0 1))"), contextBindings));
	AssertEq(False(), Eval(ParseSingle("(and #f #t)"), contextBindings));
	AssertEq(False(), Eval(ParseSingle("(and #f (error \"lalala\"))"), contextBindings));
}

TEST(AndErrorIn1stArgument) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(and (+ 1) 2)"), contextBindings));
}

TEST(Or) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(1), Eval(ParseSingle("(or #f 1)"), contextBindings));
	AssertEq(Number(1), Eval(ParseSingle("(or #f (+ 0 1))"), contextBindings));
	AssertEq(True(), Eval(ParseSingle("(or #t #f)"), contextBindings));
	AssertEq(True(), Eval(ParseSingle("(or #t (error \"lalala\"))"), contextBindings));
	AssertEq(Number(1), Eval(ParseSingle("(or 1 (error \"lalala\"))"), contextBindings));
}

TEST(OrErrorIn1stArgument) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(or (+ 1) 1)"), contextBindings));
}

TEST(If) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(0), Eval(ParseSingle("(if (and #t #f) (error \"lalala\") (+ 0 0))"), contextBindings));
	AssertEq(Number(1), Eval(ParseSingle("(if (or #t #f) (+ 0 1) (error \"lalala\"))"), contextBindings));
}

TEST(IfErrorInCondition) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(if (+ 1) 1 2)"), contextBindings));
}