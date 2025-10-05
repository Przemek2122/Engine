// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "Test/Samples/TestPerformance.h"

#if ENGINE_TESTS_ALLOW_ANY

FTestPerformance::FTestPerformance()
	: StartNS(0)
	, EndNS(0)
{
}

void FTestPerformance::DoTest()
{
	StartNS = FUtil::GetNanoSeconds();

	constexpr Uint32 NumOfIterations = 1000000;

	for (Uint32 i = 0; i < NumOfIterations; i++)
	{
		Uint64 TestNumber = 123;

		TestNumber *= 111;

		TestNumber = FMath::CeilToInt(static_cast<float>(TestNumber) / static_cast<float>(124));

		TestNumber += 172341234;

		auto NewObj = new int[32];
		delete[] NewObj;
	}

	EndNS = FUtil::GetNanoSeconds();

	const Uint64 ElapsedNS = EndNS - StartNS;
	const double Result = FUtil::NanoSecondToSecond(ElapsedNS);

	LOG_INFO("PerformanceTest result: " << Result);
}

char* FTestPerformance::GetTestName() const
{
	return TEXT_CHAR("FTestPerformance");
}

#endif