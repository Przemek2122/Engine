// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "Test/TestSingleRun.h"

#if ENGINE_TESTS_ALLOW_ANY

class FTestTypes : public FSingleRunTest
{
public:
	void DoTest() override;

	_NODISCARD char* GetTestName() const override;
		
};

#endif
