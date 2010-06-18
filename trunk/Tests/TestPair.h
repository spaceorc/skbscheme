#include "Test.h"

TEST(PairCons) {
	Term * z = FunctionCons(MakeList(2, Number(1), Number(2)));
    AssertTag(terPair, z);
	AssertEq(Number(1), z->pair->first);
	AssertEq(Number(2), z->pair->second);
}

TEST(PairConsWithOneArgument) {
	Term * z = FunctionCons(MakeList(1, Number(1)));
    AssertTag(terError, z);
}

TEST(PairConsWithThreeArguments) {
	Term * z = FunctionCons(MakeList(3, Number(1), Number(2), Number(3)));
    AssertTag(terError, z);
}

TEST(PairConsWithErrorInside) {
	Term * z = FunctionCons(MakeList(2, AllocateTerm(terError), Number(1)));
    AssertTag(terError, z);
}

TEST(PairCar) {
	Term * z = FunctionCar(MakeList(1, FunctionCons(MakeList(2, Number(1), Number(2)))));
	AssertEq(Number(1), z);
}

TEST(PairCarEmptyList) {
	Term * z = FunctionCar(MakeList(1, Nil()));
	AssertTag(terError, z);
}

TEST(PairCdr) {
	Term * z = FunctionCdr(MakeList(1, FunctionCons(MakeList(2, Number(1), Number(2)))));
	AssertEq(Number(2), z);
}

TEST(PairCdrEmptyList) {
	Term * z = FunctionCdr(MakeList(1, Nil()));
	AssertTag(terError, z);
}