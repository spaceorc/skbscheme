#include "Test.h"
#include "TestStr.h"
#include "TestTokenizer.h"
#include "TestPair.h"
#include "TestNumber.h"
#include "TestRedex.h"
#include "TestParser.h"
#include "TestDefinitions.h"
#include "TestConditionals.h"
#include <stdio.h>

void main() {
	TestFixture::RunTests();
}