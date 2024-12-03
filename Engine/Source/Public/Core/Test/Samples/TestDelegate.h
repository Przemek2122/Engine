// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"

#if ENGINE_TESTS_ALLOW_ANY
#include "Types/Delegate/DelegateSafe.h"
#include "Test/TestSingleRun.h"

class FTestDelegate : public FSingleRunTest
{
public:
	void DoTest() override;

	_NODISCARD char* GetTestName() const override;

protected:
	void TestFirstDelegate();
	void TestSecondDelegate();
	void TestThirdDelegate();
	void TestFirstSafeDelegate();
	void TestFirstSafeDelegateObjectCall(int SomeInt, float SomeFloat);

protected:
	FDelegate<> OnFirstDelegate;
	FDelegate<void, int> OnSecondDelegate;
	FDelegate<void, int, float> OnThirdDelegate;
	FDelegateSafe<void, int, float> OnFirstSafeDelegate;
	
};

#endif
