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

TEST(Cond) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(1), Eval(ParseSingle("(cond (#t 1))"), contextBindings));
	AssertEq(Number(1), Eval(ParseSingle("(cond (#t 2 1))"), contextBindings));
	AssertEq(Number(2), Eval(ParseSingle("(cond (#f 1) (#t 2))"), contextBindings));
	AssertEq(Empty(), Eval(ParseSingle("(cond (#f 1) (#f 2))"), contextBindings));
	AssertEq(Number(1), Eval(ParseSingle("(cond (1))"), contextBindings));
}

TEST(CondErrorInCondition) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(cond ((+ 1) 1))"), contextBindings));
}

TEST(CondBadSyntax) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(BadSyntax(), Eval(ParseSingle("(cond (#f 1) 2)"), contextBindings));
	AssertEq(BadSyntax(), Eval(ParseSingle("(cond ())"), contextBindings));
	AssertEq(Number(1), Eval(ParseSingle("(cond (#t 1) 2)"), contextBindings)); // todo plt says "bad syntax" ???
}

TEST(CondElse) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(Number(1), Eval(ParseSingle("(cond (else 1))"), contextBindings));
	AssertEq(Number(3), Eval(ParseSingle("(cond (#f 1) (#f 2) (else 3))"), contextBindings));
}

TEST(CondElseBadSyntax) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(True(), Eval(ParseSingle("(cond (#f 1) (else))"), contextBindings)); // todo plt says "bad syntax" ???
	AssertEq(Number(1), Eval(ParseSingle("(cond (#t 1) (else 3) (#t 4))"), contextBindings)); // todo plt says "bad syntax" ???
}