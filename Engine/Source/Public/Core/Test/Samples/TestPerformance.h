// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

#if ENGINE_TESTS_ALLOW_ANY
#include "Types/Delegate/DelegateSafe.h"
#include "Test/TestSingleRun.h"

class FTestPerformance : public FSingleRunTest
{
public:
	FTestPerformance();

	void DoTest() override;

	_NODISCARD char* GetTestName() const override;

protected:
	Uint64 StartNS, EndNS;
	
};

#endif
