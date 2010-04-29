#pragma once

#include "Number.h"

void TestNumberPlus() {
	Term * z = OperatorPlus(Cons(Number(1), Cons(Number(2), Nil()))->pair);
    assert((z->tag == tagNumber) && (z->number == 3));
}

void TestNumberPlus3Arguments() {
	Term * z = OperatorPlus(Cons(Number(1), Cons(Number(2), Cons(Number(3), Nil())))->pair);
    assert((z->tag == tagNumber) && (z->number == 6));
}

void TestNumberPlus1stIsNotANumber() {
	Term * z = OperatorPlus(Cons(Nil(), Nil())->pair);
    assert(z->tag == tagError);
}

void TestNumberPlusNotAList() {
	Term * z = OperatorPlus(Cons(Number(1), Number(2))->pair);
    assert(z->tag == tagError);
}

void TestNumberPlus3Arguments2ndIsNotANumber() {
	Term * z = OperatorPlus(Cons(Number(1), Cons(Nil(), Cons(Number(3), Nil())))->pair);
    assert(z->tag == tagError);
}

void TestNumber() {
	TestNumberPlus();
	TestNumberPlus3Arguments();
	TestNumberPlus1stIsNotANumber();
	TestNumberPlusNotAList();
	TestNumberPlus3Arguments2ndIsNotANumber();
}