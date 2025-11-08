// Created by https://www.linkedin.com/in/przemek2122/ 2020

#pragma once

#include "CoreMinimal.h"
#include "Test/TestSingleRun.h"

#if ENGINE_TESTS_ALLOW_ANY

class FTestTypes : public FSingleRunTest
{
public:
	void DoTest() override;

	NO_DISCARD char* GetTestName() const override;
		
};

#endif
