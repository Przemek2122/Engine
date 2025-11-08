// Created by https://www.linkedin.com/in/przemek2122/ 2024

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

	NO_DISCARD char* GetTestName() const override;

protected:
	Uint64 StartNS, EndNS;
	
};

#endif
