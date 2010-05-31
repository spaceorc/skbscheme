#pragma once

#include "Test.h"

TEST(PairCons) {
	Term * z = FunctionCons(List(2, Number(1), Number(2)));
    AssertTag(tagPair, z);
	AssertEq(Number(1), z->pair->first);
	AssertEq(Number(2), z->pair->second);
}

TEST(PairConsWithOneArgument) {
	Term * z = FunctionCons(List(1, Number(1)));
    AssertTag(tagError, z);
}

TEST(PairConsWithThreeArguments) {
	Term * z = FunctionCons(List(3, Number(1), Number(2), Number(3)));
    AssertTag(tagError, z);
}

TEST(PairConsWithErrorInside) {
	Term * z = FunctionCons(List(2, AllocateTerm(tagError), Number(1)));
    AssertTag(tagError, z);
}

TEST(PairCar) {
	Term * z = FunctionCar(List(1, FunctionCons(List(2, Number(1), Number(2)))));
	AssertEq(Number(1), z);
}

TEST(PairCarEmptyList) {
	Term * z = FunctionCar(List(1, Nil()));
	AssertTag(tagError, z);
}

TEST(PairCdr) {
	Term * z = FunctionCdr(List(1, FunctionCons(List(2, Number(1), Number(2)))));
	AssertEq(Number(2), z);
}

TEST(PairCdrEmptyList) {
	Term * z = FunctionCdr(List(1, Nil()));
	AssertTag(tagError, z);
}


TEST(PairApply) {
	Term * z = FunctionApply(List(3, Function(OperatorPlus), Number(1), Number(2)));
	AssertEq(Number(3), z);
}