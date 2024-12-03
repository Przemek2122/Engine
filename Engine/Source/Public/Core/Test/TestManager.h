// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "TestBase.h"

#if ENGINE_TESTS_ALLOW_ANY

/**
 * GEngine class used to check if things work properly.
 * Also can be used to see sample code.
 */
class FTestManager
{
public:
	FTestManager();
	virtual ~FTestManager();

	/** Creates Test by class. */
	template<class FTestCaseClass>
	FTestCaseClass* SpawnTestCaseByClass()
	{
		FTestCaseClass* NewTestCase = new FTestCaseClass();

		AddTest(NewTestCase);

		return NewTestCase;
	}

	/** Destroys all tests matching class. */
	template<class FTestCaseClass>
	void DestroyTestCaseByClass()
	{
		for (int i = 0; i < RunningTests.Size(); i++)
		{
			if (typeid(RunningTests[i]) == typeid(FTestCaseClass))
			{
				FTestBase* Test = RunningTests[i];
				
				RemoveTest(Test);

				delete Test;
			}
		}
	}

	virtual void AddTest(FTestBase* InTestCase, const bool bAutoStartTest = true);
	virtual void RemoveTest(FTestBase* InTestCase);
	
	virtual void Tick();
	virtual void Render();

protected:
	CArray<FTestBase*> RunningTests;
	
};

#endif
