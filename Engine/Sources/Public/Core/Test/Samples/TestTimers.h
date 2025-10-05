// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

#if ENGINE_TESTS_ALLOW_ANY
#include "Test/TestLongRun.h"

class FTimersLongRunTest : public FLongRunTest
{
public:
	FTimersLongRunTest();

	void InitializeTest() override;

	char* GetTestName() const override;

private:
	void StartTimer1();
	void StartTimer2();

	void CheckIfAllTimersFinishedAndFinishTest();

protected:
	std::shared_ptr<FTimer> TemporaryTimer1;
	std::shared_ptr<FTimer> TemporaryTimer2;

	int CurrentNumberOfTimer2Runs;
	int MaxNumberOfTimer2Runs;

};
#endif