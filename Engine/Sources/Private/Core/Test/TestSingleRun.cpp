// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"

#if ENGINE_TESTS_ALLOW_ANY
#include "Test/TestSingleRun.h"

FSingleRunTest::FSingleRunTest()
{
}

void FSingleRunTest::StartTest()
{
	FTestBase::StartTest();

	LOG_DEBUG(GetTestName() << " - test starting");

	const size_t Nanosecond_Start = FUtil::GetNanoSeconds();

	DoTest();

	const size_t Nanosecond_End = FUtil::GetNanoSeconds();
	const size_t Nanosecond_TestDuration = Nanosecond_End - Nanosecond_Start;
	const std::string ActualTimeString = std::to_string(FUtil::NanoSecondToSecond(Nanosecond_TestDuration));

	LOG_DEBUG(GetTestName() << " - Test duration(nanoseconds) : " + ActualTimeString + "s.");

	if (bAutoDeleteTest)
	{
		DestroyTest();
	}
}
#endif
