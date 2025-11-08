// Created by https://www.linkedin.com/in/przemek2122/ 2024

#include "CoreEngine.h"

#if ENGINE_TESTS_ALLOW_ANY
#include "Test/TestLongRun.h"

FLongRunTest::FLongRunTest()
{
	bAutoDeleteTest = false;
}

void FLongRunTest::StartTest()
{
	FTestBase::StartTest();

	LOG_DEBUG(GetTestName() << " - Long run test starting ...");

	InitializeTest();

	if (bAutoDeleteTest)
	{
		DestroyTest();
	}
}

void FLongRunTest::EndTask()
{
	LOG_DEBUG(GetTestName() << " - finished.");

	DestroyTest();
}
#endif
