#include "Test.h"

TEST(True) {
	Term * z = True();
	AssertTag(terBoolean, z);
	AssertThat(1 == z->boolean);
	AssertThat(IsTrue(z));
	AssertThat(!IsFalse(z));
}

TEST(False) {
	Term * z = False();
	AssertTag(terBoolean, z);
	AssertThat(0 == z->boolean);
	AssertThat(!IsTrue(z));
	AssertThat(IsFalse(z));
}

TEST(AnythingExceptFalseIsTrue) {
	AssertThat(IsTrue(Number(1)));
	AssertThat(!IsFalse(Number(1)));
	AssertThat(IsTrue(Number(0)));
	AssertThat(!IsFalse(Number(0)));
}