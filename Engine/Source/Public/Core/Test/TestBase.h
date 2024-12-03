// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"

#if ENGINE_TESTS_ALLOW_ANY

class FTestManager;

/**
 * Base class for testing.
 * Do once test.
 */
class FTestBase
{
	friend FTestManager;
	
public:
	FTestBase();
	virtual ~FTestBase();

	_NODISCARD bool IsTestDone() const;
	_NODISCARD bool ShouldAutoStartTest() const;

	/** Called once added into manager. */
	virtual void StartTest();

	void DestroyTest();
	
	_NODISCARD virtual char* GetTestName() const;

protected:
	void RegisterTest(FTestManager* InTestManager);
	void UnRegisterTest();

protected:
	bool bIsTestDone;
	bool bAutoStartTest;
	bool bAutoDeleteTest;

	FTestManager* TestManager;
	
};

#endif
