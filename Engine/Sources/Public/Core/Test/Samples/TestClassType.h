#pragma once

#include "CoreMinimal.h"
#include "Test/TestSingleRun.h"

#if ENGINE_TESTS_ALLOW_ANY
class FTemporaryTestClass
{
public:
	FTemporaryTestClass()
	{
		LOG_INFO("Temporary test class created.");
	}

	~FTemporaryTestClass()
	{
		LOG_INFO("Temporary test class destroyed.");
	}

};

class FTestClassType : public FSingleRunTest
{
public:
	void DoTest() override;
	char* GetTestName() const override;

protected:
	FClassStorage<FTemporaryTestClass> StoredClass;

};

#endif // ENGINE_TESTS_ALLOW_ANY