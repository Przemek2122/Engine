// Created by https://www.linkedin.com/in/przemek2122/ 2024

#pragma once

#include "CoreMinimal.h"

#if ENGINE_TESTS_ALLOW_ANY
#include "Test/TestBase.h"

/**
 * Any test which is not done within single function scope.
 * For example timers test where it's async.
 */
class FLongRunTest : public FTestBase
{
public:
	FLongRunTest();

	void StartTest() override;

	void EndTask();

	/** Test should be performed in this function. */
	virtual void InitializeTest() = 0;

};
#endif
