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

TEST(EvalOfOperatorEq) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(False(), Eval(ParseSingle("(= 1 2)"), contextBindings)); 
}