#pragma once

#include "Test.h"

TEST(PairCons) {
	Term * z = FunctionCons(List(2, Number(1), Number(2)));
    AssertTag(tagPair, z);
	AssertEq(Number(1), z->pair->first);
	AssertEq(Number(2), z->pair->second);
}