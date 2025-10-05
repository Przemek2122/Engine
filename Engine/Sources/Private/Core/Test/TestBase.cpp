// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Test/TestBase.h"
#include "Test/TestManager.h"

#if ENGINE_TESTS_ALLOW_ANY

FTestBase::FTestBase()
	: bIsTestDone(false)
	, bAutoStartTest(true)
	, bAutoDeleteTest(true)
	, TestManager(nullptr)
{
}

FTestBase::~FTestBase()
{
	UnRegisterTest();
}

bool FTestBase::IsTestDone() const
{
	return bIsTestDone;
}

bool FTestBase::ShouldAutoStartTest() const
{
	return bAutoStartTest;
}

void FTestBase::StartTest()
{
}

void FTestBase::DestroyTest()
{
	LOG_DEBUG(GetTestName() << " - Test auto-deleted.");

	delete this;
}

char* FTestBase::GetTestName() const
{
	return TEXT_CHAR("Unnamed test case.");
}

void FTestBase::RegisterTest(FTestManager* InTestManager)
{
	TestManager = InTestManager;
}

void FTestBase::UnRegisterTest()
{
	if (TestManager != nullptr)
	{
		TestManager->RemoveTest(this);
	}
}

#endif
