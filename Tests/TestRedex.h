#pragma once

#include "Test.h"

TEST(GeneralApply) {
	Term * z = FunctionApply(MakeList(3, Function(OperatorPlus), Number(1), Number(2)));
	AssertEq(Number(3), z);
}

TEST(GeneralApplyNotAFunction) {
	Term * z = FunctionApply(MakeList(3, Number(0), Number(1), Number(2)));
	AssertTag(tagError, z);
}
