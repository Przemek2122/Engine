// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

#if ENGINE_TESTS_ALLOW_ANY
#include "Test/TestBase.h"

/**
 * Any test with purpose of fire and forget
 */
class FSingleRunTest : public FTestBase
{
public:
	FSingleRunTest();

	void StartTest() override;

	/** Test should be performed in this function. */
	virtual void DoTest() = 0;

};
#endif
